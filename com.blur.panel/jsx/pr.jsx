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

function pr_getActiveSequence() {
    try {
        var seq = app.project.activeSequence;
        if (!seq) {
            return _pr_err('No sequence is active. Open a sequence and try again.');
        }
        var fps  = 254016000000 / seq.timebase;
        var name = _pr_esc(seq.name);
        return '{"name":"' + name + '","fps":' + fps + ',"width":' + seq.frameSizeHorizontal + ',"height":' + seq.frameSizeVertical + '}';
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

        // Find the lowest empty video track (V1, V2, V3, …) so the blurred
        // clip lands on its own layer on top of existing footage instead of
        // pushing/appending. overwriteClip is used (rather than insertClip)
        // to avoid shifting timeline content even if a track is unexpectedly
        // non-empty. Time is in ticks as a string per the Pr scripting docs.
        var tracks = seq.videoTracks;
        var n = tracks.numTracks;
        var targetIdx = -1;
        for (i = 0; i < n; i++) {
            if (tracks[i].clips.numItems === 0) {
                targetIdx = i;
                break;
            }
        }
        if (targetIdx === -1) {
            return _pr_err('All ' + n + ' video tracks are occupied. Add an empty video track above the topmost one and try again.');
        }

        tracks[targetIdx].overwriteClip(clip, '0');
        return '{"success":true,"trackIndex":' + targetIdx + '}';
    } catch (e) {
        return _pr_err('pr_importToSequence: ' + e);
    }
}
