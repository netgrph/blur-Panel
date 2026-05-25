/**
 * pr.jsx — Premiere Pro host functions
 * ES3 only.
 */

/**
 * Returns info about the currently active sequence.
 * Pr stores time as ticks; 254016000000 ticks = 1 second.
 * FPS = 254016000000 / sequence.timebase
 *
 * @returns {string} JSON: {name, fps, width, height} | {error}
 */
function pr_getActiveSequence() {
    var seq = app.project.activeSequence;
    if (!seq) {
        return JSON.stringify({ error: "No sequence is active. Open a sequence and try again." });
    }

    var fps = 254016000000 / seq.timebase;
    return JSON.stringify({
        name:   seq.name,
        fps:    fps,
        width:  seq.frameSizeHorizontal,
        height: seq.frameSizeVertical
    });
}

/**
 * Queues the active sequence for export to AME and starts the batch.
 * Returns immediately (export is asynchronous). The panel JS polls for
 * the output file's existence to know when AME finishes.
 *
 * @param {string} outputPath - Absolute path for the exported file.
 * @returns {string} JSON: {success:true, jobID} | {error}
 */
function pr_exportSequence(outputPath) {
    var seq = app.project.activeSequence;
    if (!seq) {
        return JSON.stringify({ error: "No sequence is active." });
    }

    var jobID = app.encoder.encodeSequence(
        seq,
        outputPath,
        "",   // use AME default preset
        app.encoder.ENCODE_ENTIRE,
        true  // remove from AME queue on completion
    );

    if (jobID === "0") {
        return JSON.stringify({ error: "AME failed to queue the export. Make sure Adobe Media Encoder is installed." });
    }

    app.encoder.startBatch();
    return JSON.stringify({ success: true, jobID: jobID });
}

/**
 * Imports a file into the project bin and inserts it at the beginning of
 * the first video track in the active sequence.
 *
 * @param {string} filePath - Absolute path to the blurred output file.
 * @returns {string} JSON: {success:true} | {error}
 */
function pr_importToSequence(filePath) {
    var seq = app.project.activeSequence;
    if (!seq) {
        return JSON.stringify({ error: "No sequence is active." });
    }

    try {
        var importResult = app.project.importFiles(
            [filePath],
            true,                    // suppressUI
            app.project.rootItem,    // destination bin
            false                    // importAsNumberedStills
        );

        if (!importResult) {
            return JSON.stringify({ error: "Import returned no result." });
        }

        // Find the imported clip in the root bin (it's the most-recently added child)
        var root = app.project.rootItem;
        var clip = null;
        var i;
        for (i = root.children.numItems - 1; i >= 0; i--) {
            var child = root.children[i];
            if (child.name === new File(filePath).name) {
                clip = child;
                break;
            }
        }

        if (!clip) {
            // Fallback: use last child
            clip = root.children[root.children.numItems - 1];
        }

        seq.videoTracks[0].insertClip(clip, 0);
        return JSON.stringify({ success: true });
    } catch (e) {
        return JSON.stringify({ error: e.toString() });
    }
}
