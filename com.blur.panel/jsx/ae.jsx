/**
 * ae.jsx - After Effects host functions
 * ES3 only. No JSON.stringify - strings are built manually for compatibility.
 */

function _ae_esc(s) {
    return String(s).replace(/\\/g, '\\\\').replace(/"/g, '\\"').replace(/\r/g, '').replace(/\n/g, ' ');
}
function _ae_ok(fields) {
    var parts = [];
    for (var k in fields) {
        if (fields.hasOwnProperty(k)) {
            var v = fields[k];
            if (typeof v === 'string') parts.push('"' + k + '":"' + _ae_esc(v) + '"');
            else parts.push('"' + k + '":' + v);
        }
    }
    return '{' + parts.join(',') + '}';
}
function _ae_err(msg) {
    return '{"error":"' + _ae_esc(msg) + '"}';
}

function ae_getActiveComp() {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return _ae_err('No composition is selected. Click a composition in the Project panel first.');
        }
        return _ae_ok({
            name:     item.name,
            fps:      item.frameRate,
            width:    item.width,
            height:   item.height,
            duration: item.duration
        });
    } catch (e) {
        return _ae_err('ae_getActiveComp: ' + e);
    }
}

function ae_preRender(outputPath) {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return _ae_err('No composition is selected.');
        }
        var rqItem = app.project.renderQueue.items.add(item);
        var om = rqItem.outputModule(1);
        om.file = new File(outputPath);
        app.project.renderQueue.render();
        // AE may change the extension to match the output module codec —
        // read back the actual path it wrote to before removing the item.
        var actualPath = om.file.fsName;
        rqItem.remove();
        var safePath = String(actualPath).replace(/\\/g, '/').replace(/"/g, "'");
        return '{"success":true,"actualPath":"' + safePath + '"}';
    } catch (e) {
        return _ae_err('ae_preRender: ' + e);
    }
}

function ae_importAndAddLayer(filePath) {
    // Self-contained: no helper function calls so nothing external can be missing.
    try {
        // After a render, activeItem may have changed — search all comps.
        var item = app.project.activeItem;
        if (!item || !(item instanceof CompItem)) {
            // Fallback: find the first open comp
            var i;
            for (i = 1; i <= app.project.numItems; i++) {
                if (app.project.item(i) instanceof CompItem) {
                    item = app.project.item(i);
                    break;
                }
            }
        }
        if (!item || !(item instanceof CompItem)) {
            return '{"error":"ae_importAndAddLayer: no CompItem found in project"}';
        }

        var f = new File(filePath);
        if (!f.exists) {
            return '{"error":"ae_importAndAddLayer: file not found: ' + filePath + '"}';
        }

        var opts    = new ImportOptions(f);
        var footage = app.project.importFile(opts);
        if (!footage) {
            return '{"error":"ae_importAndAddLayer: importFile returned null"}';
        }
        var layer = item.layers.add(footage);
        layer.moveToBeginning();
        return '{"success":true}';
    } catch (e) {
        var msg = String(e).replace(/\\/g, '/').replace(/"/g, "'");
        return '{"error":"ae_importAndAddLayer caught: ' + msg + '"}';
    }
}
