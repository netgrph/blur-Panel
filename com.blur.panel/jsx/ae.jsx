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

// useLossless=true  → apply Lossless/Verlustfrei template (AVI)
// useLossless=false → apply best stock H.264 template, then patch bitrate to 300 Mbps
//                     on the live OM via setSettings(STRING_SETTABLE). Nothing is saved
//                     as a template — settings die with the RQ item when we remove it.
function ae_preRender(outputPath, useLossless) {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return _ae_err('No composition is selected.');
        }
        var rqItem = app.project.renderQueue.items.add(item);
        var om = rqItem.outputModule(1);
        var _tplInfo = 'skipped';
        var _tplList, _ti;
        if (useLossless) {
            var _lang   = String($.locale || 'en').substring(0, 2).toLowerCase();
            var _tplMap = { de: 'Verlustfrei', fr: 'Sans perte', es: 'Sin perdida',
                            ja: 'ロスレス', ko: '무손실', zh: '无损' };
            _tplList = [_tplMap[_lang] || 'Lossless', 'Lossless', 'Verlustfrei'];
        } else {
            _tplList = [
                'H.264 4K',
                'H.264 - Match Render Settings - 15 Mbps',
                'H.264',
                'H.264 - Low Complexity'
            ];
        }
        for (_ti = 0; _ti < _tplList.length; _ti++) {
            try { om.applyTemplate(_tplList[_ti]); _tplInfo = 'ok:' + _tplList[_ti]; break; }
            catch (e2) { _tplInfo = 'err(' + _tplList[_ti] + '):' + String(e2).substring(0, 80); }
        }

        // H.264 path: boost bitrate to 300 Mbps inline on the live OM. Silently no-ops
        // if AE's settings format doesn't match — render proceeds at base-template bitrate.
        if (!useLossless) {
            try {
                var _s = om.getSettings(OMSettingsFormat.STRING_SETTABLE);
                if (typeof _s === 'string' && _s.length > 0) {
                    _s = _s.replace(/(Target Bitrate \[Mbps\])[^\n\r]*/g, '$1: 300');
                    _s = _s.replace(/(Max Bitrate \[Mbps\])[^\n\r]*/g,    '$1: 300');
                    try { om.setSettings(OMSettingsFormat.STRING_SETTABLE, _s); } catch (e3) {}
                }
            } catch (e4) {}
        }

        om.file = new File(outputPath);
        app.project.renderQueue.render();
        // AE may change the extension to match the output module codec —
        // read back the actual path it wrote to before removing the item.
        var actualPath = om.file.fsName;
        rqItem.remove();
        var safePath = String(actualPath).replace(/\\/g, '/').replace(/"/g, "'");
        return '{"success":true,"actualPath":"' + safePath + '","tpl":"' + _ae_esc(_tplInfo) + '"}';
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
