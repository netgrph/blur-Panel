/**
 * pr.jsx - Premiere Pro host functions
 * ES3 only. No JSON.stringify - strings are built manually for compatibility.
 */

function _pr_esc(s) {
    return String(s).replace(/\\/g, '\\\\').replace(/"/g, '\\"').replace(/\r/g, '').replace(/\n/g, ' ');
}
function _pr_err(msg) {
    return '{"error":"' + _pr_esc(msg) + '"}';
}

// Add exactly one new track of the requested kind (addV=1 XOR addA=1) via
// the QE DOM. Brute-forces insertion-index, channel-type, and arg-count
// variants of qeSeq.addTracks() because the signature varies across Pr
// versions and many builds silently no-op all but one specific combo.
// Verifies growth after every attempt against BOTH the regular DOM and
// QE-side numTracks (regular DOM lags QE in some builds), re-fetches seq
// and qeSeq on each pass to dodge cached numTracks, and stops on the first
// verified add so extras can't accumulate.
function _pr_addOneTrack(seq, qeSeq, addV, addA) {
    var beforeV   = seq.videoTracks.numTracks;
    var beforeA   = seq.audioTracks.numTracks;
    var qeBeforeV = (typeof qeSeq.numVideoTracks === 'number') ? qeSeq.numVideoTracks : -1;
    var qeBeforeA = (typeof qeSeq.numAudioTracks === 'number') ? qeSeq.numAudioTracks : -1;
    var base = (addV > 0) ? beforeV : beforeA;
    var argCounts = [8, 7, 5];
    var at, ch, gi, qeNowV, qeNowA, grewV, grewA, vAtTry, aAtTry, argLen;
    for (at = base + 1; at >= 0; at--) {
        vAtTry = (addV > 0) ? at : 0;
        aAtTry = (addA > 0) ? at : 0;
        for (ch = 0; ch <= 4; ch++) {
            for (gi = 0; gi < argCounts.length; gi++) {
                argLen = argCounts[gi];
                try {
                    if (argLen === 8)      qeSeq.addTracks(addV, vAtTry, addA, aAtTry, ch, 0, 0, ch);
                    else if (argLen === 7) qeSeq.addTracks(addV, vAtTry, addA, aAtTry, ch, 0, 0);
                    else                   qeSeq.addTracks(addV, vAtTry, addA, aAtTry, ch);
                } catch (eA) { continue; }
                try { seq   = app.project.activeSequence; } catch (eRr) {}
                try { qeSeq = qe.project.getActiveSequence(); } catch (eRq) {}
                qeNowV = (qeSeq && typeof qeSeq.numVideoTracks === 'number') ? qeSeq.numVideoTracks : -1;
                qeNowA = (qeSeq && typeof qeSeq.numAudioTracks === 'number') ? qeSeq.numAudioTracks : -1;
                grewV  = (seq.videoTracks.numTracks > beforeV) || (qeNowV > qeBeforeV);
                grewA  = (seq.audioTracks.numTracks > beforeA) || (qeNowA > qeBeforeA);
                if ((addV === 0 || grewV) && (addA === 0 || grewA)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Parent directory of a path string — ES3-safe, handles both / and \.
function _pr_parentDir(p) {
    if (!p) return '';
    var s = String(p);
    var lastBs = s.lastIndexOf('\\');
    var lastFs = s.lastIndexOf('/');
    var lastSep = (lastBs > lastFs) ? lastBs : lastFs;
    if (lastSep <= 0) return '';
    return s.substring(0, lastSep);
}

function pr_getActiveSequence() {
    try {
        var seq = app.project.activeSequence;
        if (!seq) {
            return _pr_err('No sequence is active. Open a sequence and try again.');
        }
        var fps  = 254016000000 / seq.timebase;
        var name = _pr_esc(seq.name);

        // Folder the .prproj is saved in — routed by the panel as the base
        // for pre-render / blur output. Empty string if project is unsaved.
        var projectFolder = '';
        try { projectFolder = _pr_parentDir(app.project.path); } catch (ep) {}

        return '{"name":"' + name +
               '","fps":' + fps +
               ',"width":' + seq.frameSizeHorizontal +
               ',"height":' + seq.frameSizeVertical +
               ',"projectFolder":"' + _pr_esc(projectFolder) + '"}';
    } catch (e) {
        return _pr_err('pr_getActiveSequence: ' + e);
    }
}

function pr_exportSequence(outputPath, presetPath) {
    try {
        var seq = app.project.activeSequence;
        if (!seq) {
            return _pr_err('No sequence is active.');
        }
        if (typeof seq.exportAsMediaDirect !== 'function') {
            return _pr_err('Sequence.exportAsMediaDirect is not available in this Premiere Pro version.');
        }
        var preset = presetPath ? presetPath : '';
        var ok;
        try {
            ok = seq.exportAsMediaDirect(outputPath, preset, 0);
        } catch (innerE) {
            return _pr_err('exportAsMediaDirect threw: ' + innerE +
                (preset ? '' : ' (called with empty preset — a .epr preset file is likely required)'));
        }
        // exportAsMediaDirect doesn't return a JS boolean despite the docs — it returns Pr's
        // EncoderHost.Error object. Its .toString() is "No Error" on success, or a real error
        // message on failure (e.g. "Preset file doesn't exist").
        var okStr = String(ok);
        var success = (ok === true) || (okStr === 'No Error') || (okStr === 'true');
        if (!success) {
            return _pr_err('exportAsMediaDirect failed: ' + okStr +
                (preset ? ' (preset: "' + preset + '")' : ' (empty preset not accepted — a .epr preset file is likely required)'));
        }
        return '{"success":true,"outputPath":"' + _pr_esc(outputPath) + '"}';
    } catch (e) {
        return _pr_err('pr_exportSequence: ' + e);
    }
}

function pr_importToSequence(filePath) {
    try {
        var seq = app.project.activeSequence;
        if (!seq) {
            return _pr_err('No sequence is active.');
        }
        var importResult = app.project.importFiles(
            [filePath],
            true,
            app.project.rootItem,
            false
        );
        if (!importResult) {
            return _pr_err('Import returned no result.');
        }
        var root = app.project.rootItem;
        var clip = null;
        var fname = new File(filePath).name;
        var i;
        for (i = root.children.numItems - 1; i >= 0; i--) {
            var child = root.children[i];
            if (child.name === fname) { clip = child; break; }
        }
        if (!clip) {
            clip = root.children[root.children.numItems - 1];
        }

        // Tag the bin item with a distinctive label color (Mango / orange)
        // so the blurred clip is easy to spot on the timeline and in the
        // project panel. Non-fatal if unsupported.
        try { clip.setColorLabel(7); } catch (eLabel) {}

        // Find the HIGHEST empty video and audio track (scan top→bottom) so
        // the blurred clip lands on the topmost free row instead of the
        // bottom-most. Placement uses Sequence.overwriteClip(item, time,
        // vIdx, aIdx) — the only API that takes both indices AND overwrites
        // rather than inserts (Track-level overwriteClip drops audio on a
        // default track, and Track/Sequence insertClip pushes existing audio
        // aside on sync-locked tracks).
        var vTracks = seq.videoTracks;
        var vN = vTracks.numTracks;
        var vIdx = -1;
        for (i = vN - 1; i >= 0; i--) {
            if (vTracks[i].clips.numItems === 0) { vIdx = i; break; }
        }
        var aTracks = seq.audioTracks;
        var aN = aTracks.numTracks;
        var aIdx = -1;
        for (i = aN - 1; i >= 0; i--) {
            if (aTracks[i].clips.numItems === 0) { aIdx = i; break; }
        }

        // If every video or audio track is occupied, append exactly one new
        // track of the missing kind via the QE DOM. The video and audio adds
        // are issued in SEPARATE calls so a single call can't accidentally
        // affect both dimensions — earlier combined calls were adding extra
        // tracks (e.g. 6 video tracks for a single-needed-video case) when
        // detection of one dimension lagged the other.
        var addedV = false, addedA = false;
        if (vIdx === -1 || aIdx === -1) {
            var qeSeq = null;
            try {
                app.enableQE();
                qeSeq = qe.project.getActiveSequence();
            } catch (eQE) {
                return _pr_err('All tracks occupied and could not enable QE DOM: ' + eQE);
            }
            if (!qeSeq) {
                return _pr_err('All tracks occupied and qe.project.getActiveSequence() returned null.');
            }

            try { app.beginUndoGroup('Blur Panel: add track'); } catch (eU) {}

            if (vIdx === -1) {
                if (!_pr_addOneTrack(seq, qeSeq, 1, 0)) {
                    try { app.endUndoGroup(); } catch (eEU1) {}
                    return _pr_err('Could not add video track via QE addTracks.');
                }
                addedV = true;
            }
            if (aIdx === -1) {
                if (!_pr_addOneTrack(seq, qeSeq, 0, 1)) {
                    try { app.endUndoGroup(); } catch (eEU2) {}
                    return _pr_err('Could not add audio track via QE addTracks.');
                }
                addedA = true;
            }

            try { app.endUndoGroup(); } catch (eEU) {}

            // Re-scan from the TOP so the imported clip lands on the
            // highest-numbered empty track (the newly-added top row in most
            // builds, regardless of where Pr actually slotted it).
            vTracks = seq.videoTracks;
            vN = vTracks.numTracks;
            if (vIdx === -1) {
                for (i = vN - 1; i >= 0; i--) {
                    if (vTracks[i].clips.numItems === 0) { vIdx = i; break; }
                }
            }
            aTracks = seq.audioTracks;
            aN = aTracks.numTracks;
            if (aIdx === -1) {
                for (i = aN - 1; i >= 0; i--) {
                    if (aTracks[i].clips.numItems === 0) { aIdx = i; break; }
                }
            }
            if (vIdx === -1 || aIdx === -1) {
                return _pr_err('addTracks reported growth but no empty track was found afterwards (vIdx=' + vIdx + ', aIdx=' + aIdx + ').');
            }
        }

        if (typeof seq.overwriteClip !== 'function') {
            return _pr_err('Sequence.overwriteClip(item,time,vIdx,aIdx) is not available in this Premiere Pro version.');
        }
        var owOk;
        try {
            owOk = seq.overwriteClip(clip, '0', vIdx, aIdx);
        } catch (eOw) {
            return _pr_err('seq.overwriteClip threw at vIdx=' + vIdx + ', aIdx=' + aIdx + ': ' + eOw);
        }
        if (owOk === false) {
            return _pr_err('seq.overwriteClip returned false at vIdx=' + vIdx + ', aIdx=' + aIdx + ' (likely channel-type mismatch on the target audio track).');
        }
        return '{"success":true,"trackIndex":' + vIdx + ',"audioTrackIndex":' + aIdx
            + ',"addedVideoTrack":' + (addedV ? 'true' : 'false')
            + ',"addedAudioTrack":' + (addedA ? 'true' : 'false') + '}';
    } catch (e) {
        return _pr_err('pr_importToSequence: ' + e);
    }
}
