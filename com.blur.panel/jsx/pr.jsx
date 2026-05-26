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

function pr_exportSequence(outputPath) {
    try {
        var seq = app.project.activeSequence;
        if (!seq) {
            return _pr_err('No sequence is active.');
        }
        var jobID = app.encoder.encodeSequence(
            seq,
            outputPath,
            '',
            app.encoder.ENCODE_ENTIRE,
            true
        );
        if (jobID === '0') {
            return _pr_err('AME failed to queue the export. Make sure Adobe Media Encoder is installed.');
        }
        app.encoder.startBatch();
        return '{"success":true,"jobID":"' + _pr_esc(jobID) + '"}';
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
        seq.videoTracks[0].insertClip(clip, 0);
        return '{"success":true}';
    } catch (e) {
        return _pr_err('pr_importToSequence: ' + e);
    }
}
