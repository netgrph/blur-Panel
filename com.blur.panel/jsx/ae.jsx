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

// ─── Bitrate-patch helpers (H.264 pre-render path) ───────────────────────────
// getSettings(STRING_SETTABLE) returns a nested object, not a string. We walk
// it recursively, find any "*Bitrate*" leaf, and build a parallel patched
// object to pass back through setSettings.

function _ae_findBitrate(obj, prefix, out) {
    var k, v, p;
    if (obj === null || typeof obj !== 'object') return;
    for (k in obj) {
        if (!obj.hasOwnProperty(k)) continue;
        v = obj[k];
        p = prefix ? (prefix + ' > ' + k) : k;
        if (k.toLowerCase().indexOf('bitrate') !== -1
            && (typeof v === 'string' || typeof v === 'number' || typeof v === 'boolean')) {
            out.push({ path: p, value: v });
        }
        if (v !== null && typeof v === 'object') {
            _ae_findBitrate(v, p, out);
        }
    }
}

function _ae_patchBitrate(obj, targetMbps) {
    var k, v, out;
    if (obj === null || typeof obj !== 'object') return obj;
    out = {};
    for (k in obj) {
        if (!obj.hasOwnProperty(k)) continue;
        v = obj[k];
        if (k.toLowerCase().indexOf('bitrate') !== -1) {
            if      (typeof v === 'number') out[k] = targetMbps;
            else if (typeof v === 'string') out[k] = String(targetMbps);
            else if (v !== null && typeof v === 'object') out[k] = _ae_patchBitrate(v, targetMbps);
            else out[k] = v;
        } else if (v !== null && typeof v === 'object') {
            out[k] = _ae_patchBitrate(v, targetMbps);
        } else {
            out[k] = v;
        }
    }
    return out;
}

function _ae_diagJson(diag) {
    var parts = [], k, v, i, item, sub;
    for (k in diag) {
        if (!diag.hasOwnProperty(k)) continue;
        v = diag[k];
        if (v === null || v === undefined) {
            parts.push('"' + k + '":null');
        } else if (typeof v === 'boolean' || typeof v === 'number') {
            parts.push('"' + k + '":' + v);
        } else if (typeof v === 'string') {
            parts.push('"' + k + '":"' + _ae_esc(v) + '"');
        } else if (v instanceof Array) {
            sub = [];
            for (i = 0; i < v.length && i < 8; i++) {
                item = v[i];
                sub.push('{"path":"' + _ae_esc(String(item.path).substring(0, 80))
                       + '","value":"' + _ae_esc(String(item.value).substring(0, 40)) + '"}');
            }
            parts.push('"' + k + '":[' + sub.join(',') + ']');
        }
    }
    return '{' + parts.join(',') + '}';
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
// useLossless=false → apply best stock H.264 template, then patch every Bitrate-named
//                     leaf in the OM settings object to a resolution-aware target via
//                     setSettings(). Nothing is saved as a template — settings die with
//                     the RQ item when we remove it. Target stays under H.264 codec
//                     level caps to avoid silent clamping.
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

        // H.264 path: patch every "*Bitrate*" leaf in the OM settings object.
        // Target chosen per resolution to stay under H.264 codec-level caps —
        // asking for more than the level allows silently clamps to default.
        //   ≤ 1920px (1080p)  → 50 Mbps   (Level 4.1)
        //   ≤ 2560px (1440p)  → 100 Mbps
        //   > 2560px (4K+)    → 240 Mbps  (Level 5.1)
        // STRING_SETTABLE = 1. The documented enum is GetSettingsFormat; some
        // AE versions also expose OMSettingsFormat as an alias. Fall back to the
        // raw integer if neither symbol is defined.
        var _SETTABLE = 1;
        try { if (typeof GetSettingsFormat !== 'undefined' && GetSettingsFormat.STRING_SETTABLE != null)
                _SETTABLE = GetSettingsFormat.STRING_SETTABLE; } catch (eEnum1) {}
        try { if (_SETTABLE === 1 && typeof OMSettingsFormat !== 'undefined' && OMSettingsFormat.STRING_SETTABLE != null)
                _SETTABLE = OMSettingsFormat.STRING_SETTABLE; } catch (eEnum2) {}

        var _diag = { target: 0, before: [], after: [], applied: false, error: null };
        if (!useLossless) {
            var _maxDim = item.width > item.height ? item.width : item.height;
            var _bitrate;
            if      (_maxDim <= 1920) _bitrate = 50;
            else if (_maxDim <= 2560) _bitrate = 100;
            else                      _bitrate = 240;
            _diag.target = _bitrate;
            try {
                var _raw = om.getSettings(_SETTABLE);
                _ae_findBitrate(_raw, '', _diag.before);
                if (_diag.before.length > 0) {
                    var _patched = _ae_patchBitrate(_raw, _bitrate);
                    try {
                        om.setSettings(_patched);
                        _diag.applied = true;
                        // AE docs: OM ref invalidates after setSettings — must re-fetch.
                        om = rqItem.outputModule(1);
                        _ae_findBitrate(om.getSettings(_SETTABLE), '', _diag.after);
                    } catch (eSet) {
                        _diag.error = 'set:' + String(eSet).substring(0, 120);
                    }
                } else {
                    _diag.error = 'no bitrate keys in settings';
                }
            } catch (eGet) {
                _diag.error = 'get:' + String(eGet).substring(0, 120);
            }
        }

        om.file = new File(outputPath);
        app.project.renderQueue.render();
        // AE may change the extension to match the output module codec —
        // read back the actual path it wrote to before removing the item.
        var actualPath = om.file.fsName;
        rqItem.remove();
        var safePath = String(actualPath).replace(/\\/g, '/').replace(/"/g, "'");
        return '{"success":true,"actualPath":"' + safePath
            + '","tpl":"' + _ae_esc(_tplInfo)
            + '","bitrate":' + _ae_diagJson(_diag) + '}';
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
