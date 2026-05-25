/**
 * ae.jsx — After Effects host functions
 * ES3 only.
 */

/**
 * Returns info about the currently active composition.
 * @returns {string} JSON: {name, fps, width, height, duration} | {error}
 */
function ae_getActiveComp() {
    var item = app.project.activeItem;
    if (item === null || !(item instanceof CompItem)) {
        return JSON.stringify({ error: "No composition is selected. Click on a composition and try again." });
    }
    return JSON.stringify({
        name:     item.name,
        fps:      item.frameRate,
        width:    item.width,
        height:   item.height,
        duration: item.duration
    });
}

/**
 * Adds the active composition to the render queue, renders to outputPath,
 * then removes the render queue item. Blocks until render is complete.
 *
 * @param {string} outputPath - Absolute path for the pre-render output file.
 * @returns {string} JSON: {success:true} | {error}
 */
function ae_preRender(outputPath) {
    var item = app.project.activeItem;
    if (item === null || !(item instanceof CompItem)) {
        return JSON.stringify({ error: "No composition is selected." });
    }

    var rqItem;
    try {
        rqItem = app.project.renderQueue.items.add(item);

        var om = rqItem.outputModule(1);
        om.file = new File(outputPath);

        app.project.renderQueue.render();

        rqItem.remove();
        return JSON.stringify({ success: true });
    } catch (e) {
        if (rqItem) {
            try { rqItem.remove(); } catch (ignore) {}
        }
        return JSON.stringify({ error: e.toString() });
    }
}

/**
 * Imports a file into the project and adds it as the top layer of the
 * currently active composition.
 *
 * @param {string} filePath - Absolute path to the blurred output file.
 * @returns {string} JSON: {success:true} | {error}
 */
function ae_importAndAddLayer(filePath) {
    var item = app.project.activeItem;
    if (item === null || !(item instanceof CompItem)) {
        return JSON.stringify({ error: "No composition is selected." });
    }

    try {
        var opts    = new ImportOptions(new File(filePath));
        var footage = app.project.importFile(opts);
        var layer   = item.layers.add(footage);
        layer.moveToBeginning();
        return JSON.stringify({ success: true, layerName: layer.name });
    } catch (e) {
        return JSON.stringify({ error: e.toString() });
    }
}
