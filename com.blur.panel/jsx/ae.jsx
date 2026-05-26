/**
 * ae.jsx — After Effects host functions
 * ES3 only.
 */

function ae_getActiveComp() {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return JSON.stringify({ error: "No composition is selected. Open a project and click on a composition first." });
        }
        return JSON.stringify({
            name:     item.name,
            fps:      item.frameRate,
            width:    item.width,
            height:   item.height,
            duration: item.duration
        });
    } catch (e) {
        return JSON.stringify({ error: "ae_getActiveComp threw: " + e.toString() });
    }
}

function ae_preRender(outputPath) {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return JSON.stringify({ error: "No composition is selected." });
        }

        var rqItem = app.project.renderQueue.items.add(item);
        var om = rqItem.outputModule(1);
        om.file = new File(outputPath);
        app.project.renderQueue.render();
        rqItem.remove();
        return JSON.stringify({ success: true });
    } catch (e) {
        return JSON.stringify({ error: "ae_preRender threw: " + e.toString() });
    }
}

function ae_importAndAddLayer(filePath) {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return JSON.stringify({ error: "No composition is selected." });
        }

        var opts    = new ImportOptions(new File(filePath));
        var footage = app.project.importFile(opts);
        var layer   = item.layers.add(footage);
        layer.moveToBeginning();
        return JSON.stringify({ success: true, layerName: layer.name });
    } catch (e) {
        return JSON.stringify({ error: "ae_importAndAddLayer threw: " + e.toString() });
    }
}
