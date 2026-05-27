/**
 * ae.jsx - After Effects host functions
 * ES3 only. No JSON.stringify - strings are built manually for compatibility.
 */

var BLUR_HQ_TEMPLATE_NAME = 'Blur Panel HQ';

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
// useLossless=false → prefer the user-created "Blur Panel HQ" template (set up via
//                     Edit > Templates > Output Module...). If missing, fall back to
//                     any stock H.264 template (typically maxes at 40 Mbps default).
// AE's scripting API has no setSettings path for video codec/bitrate, so a custom
// template the user defines once is the only way to get higher bitrates programmatically.
function ae_preRender(outputPath, useLossless) {
    try {
        var item = app.project.activeItem;
        if (item === null || item === undefined || !(item instanceof CompItem)) {
            return _ae_err('No composition is selected.');
        }
        var rqItem = app.project.renderQueue.items.add(item);
        var om = rqItem.outputModule(1);
        var tplInfo = 'skipped';
        var hasHqTemplate = false;
        var availTpls = [];
        try { availTpls = om.templates || []; } catch (eList) { availTpls = []; }

        var i, nm, lc;

        // 1) Prefer the user-created HQ template
        if (!useLossless) {
            for (i = 0; i < availTpls.length; i++) {
                if (String(availTpls[i]) === BLUR_HQ_TEMPLATE_NAME) {
                    hasHqTemplate = true;
                    break;
                }
            }
            if (hasHqTemplate) {
                try {
                    om.applyTemplate(BLUR_HQ_TEMPLATE_NAME);
                    tplInfo = 'hq:' + BLUR_HQ_TEMPLATE_NAME;
                } catch (eHq) {
                    tplInfo = 'err(hq):' + String(eHq).substring(0, 80);
                    hasHqTemplate = false;
                }
            }
        }

        // 2) Fallback — locale-aware substring match against stock templates
        if (!hasHqTemplate) {
            var tplList = [];
            for (i = 0; i < availTpls.length; i++) {
                nm = String(availTpls[i]);
                lc = nm.toLowerCase();
                if (useLossless) {
                    if (lc.indexOf('lossless')        !== -1 ||  // English
                        lc.indexOf('verlustfrei')     !== -1 ||  // German
                        lc.indexOf('sans perte')      !== -1 ||  // French
                        lc.indexOf('senza perdita')   !== -1 ||  // Italian
                        lc.indexOf('sin perdida')     !== -1 ||  // Spanish (sin pérdida — diacritic stripped by toLowerCase below isn't reliable; match both)
                        lc.indexOf('sin pérdida')     !== -1 ||  // Spanish (with diacritic)
                        lc.indexOf('sem perdas')      !== -1 ||  // Portuguese
                        lc.indexOf('bezstratny')      !== -1 ||  // Polish (bezstratny / bezstratna)
                        lc.indexOf('bezztrátový')     !== -1 ||  // Czech
                        lc.indexOf('bezztratovy')     !== -1 ||  // Czech (diacritic-less variant)
                        lc.indexOf('kayipsiz')        !== -1 ||  // Turkish (diacritic-less)
                        lc.indexOf('kayıpsız')        !== -1 ||  // Turkish
                        nm.indexOf('без потерь')      !== -1 ||  // Russian
                        nm.indexOf('ロスレス')         !== -1 ||  // Japanese
                        nm.indexOf('무손실')           !== -1 ||  // Korean
                        nm.indexOf('无损')             !== -1 ||  // Chinese (Simplified)
                        nm.indexOf('無損')             !== -1 ||  // Chinese (Traditional)
                        nm.indexOf('無失真')           !== -1) {  // Chinese (Traditional, alt term)
                        tplList.push(nm);
                    }
                } else {
                    if (lc.indexOf('h.264') !== -1 || lc.indexOf('h264') !== -1) {
                        tplList.push(nm);
                    }
                }
            }
            // Prefer 4K-named H.264 templates first (higher base bitrate cap)
            if (!useLossless) {
                tplList.sort(function(a, b) {
                    var aK = String(a).toLowerCase().indexOf('4k') !== -1 ? 0 : 1;
                    var bK = String(b).toLowerCase().indexOf('4k') !== -1 ? 0 : 1;
                    return aK - bK;
                });
            }
            for (i = 0; i < tplList.length; i++) {
                try {
                    om.applyTemplate(tplList[i]);
                    tplInfo = 'fallback:' + tplList[i];
                    break;
                } catch (eApply) {
                    tplInfo = 'err(' + tplList[i] + '):' + String(eApply).substring(0, 60);
                }
            }
            if (tplList.length === 0) tplInfo = 'no-match(' + availTpls.length + ' avail)';
        }

        om.file = new File(outputPath);
        app.project.renderQueue.render();
        // AE may change the extension to match the output module codec —
        // read back the actual path it wrote to before removing the item.
        var actualPath = om.file.fsName;
        rqItem.remove();
        var safePath = String(actualPath).replace(/\\/g, '/').replace(/"/g, "'");
        return '{"success":true,"actualPath":"' + safePath
            + '","tpl":"' + _ae_esc(tplInfo)
            + '","hasHqTemplate":' + (hasHqTemplate ? 'true' : 'false') + '}';
    } catch (e) {
        return _ae_err('ae_preRender: ' + e);
    }
}

// Probe whether "Blur Panel HQ" exists without rendering. Requires any comp in the
// project (active preferred); wraps in an undo group so the temporary RQ item is
// one consolidated undo step the user can dismiss.
function ae_hasHqTemplate() {
    try {
        var item = app.project.activeItem;
        var i;
        if (!item || !(item instanceof CompItem)) {
            for (i = 1; i <= app.project.numItems; i++) {
                if (app.project.item(i) instanceof CompItem) { item = app.project.item(i); break; }
            }
        }
        if (!item || !(item instanceof CompItem)) {
            return '{"success":true,"present":false,"reason":"no comp"}';
        }
        app.beginUndoGroup('Blur Panel: template probe');
        var probeItem = app.project.renderQueue.items.add(item);
        var om = probeItem.outputModule(1);
        var arr = [];
        try { arr = om.templates || []; } catch (eT) {}
        var present = false;
        for (i = 0; i < arr.length; i++) {
            if (String(arr[i]) === BLUR_HQ_TEMPLATE_NAME) { present = true; break; }
        }
        probeItem.remove();
        app.endUndoGroup();
        return '{"success":true,"present":' + (present ? 'true' : 'false')
            + ',"templateCount":' + arr.length + '}';
    } catch (e) {
        return _ae_err('ae_hasHqTemplate: ' + e);
    }
}

// Open AE's "Edit > Templates > Output Module..." dialog. app.executeCommand
// silently no-ops on unknown IDs and the ID varies across AE versions, so we
// resolve it dynamically via findMenuCommandId() with localized labels, then
// fall back to historical numeric IDs. Returns which method/id actually worked
// (or {error} if nothing was found) so the panel can fall back to instructions.
function ae_openOMTemplateEditor() {
    try {
        // Bring AE to the foreground so the dialog isn't hidden behind the panel.
        try { app.activate(); } catch (eAct) {}
        // Adobe AE ships in these locales; labels below match the dialog
        // entry "Edit > Templates > Output Module..." in each. Numeric-ID
        // fallback below catches any locale we miss.
        var labels = [
            'Output Module...',         // English
            'Ausgabemodul...',          // German
            'Module de sortie...',      // French
            'Modulo di output...',      // Italian
            'Módulo de salida...',      // Spanish
            'Módulo de saída...',       // Portuguese (Brazil)
            'Moduł wyjściowy...',       // Polish
            'Výstupní modul...',        // Czech
            'Çıktı Modülü...',          // Turkish
            'Модуль вывода...',         // Russian
            '出力モジュール...',         // Japanese
            '输出模块...',                // Chinese (Simplified)
            '輸出模組...',                // Chinese (Traditional)
            '출력 모듈...'                // Korean
        ];
        var i, cid;
        for (i = 0; i < labels.length; i++) {
            try {
                cid = app.findMenuCommandId(labels[i]);
                if (cid && cid > 0) {
                    app.executeCommand(cid);
                    return '{"success":true,"id":' + cid + ',"method":"findMenuCommandId","label":"' + _ae_esc(labels[i]) + '"}';
                }
            } catch (e1) {}
        }
        // Fallback: known historical IDs across AE versions. executeCommand
        // silently no-ops on bad IDs so we can't tell which actually worked —
        // but firing all of them is harmless (no dialog opens for wrong IDs).
        var fallbacks = [3075, 2360, 2359, 3076, 2361];
        for (i = 0; i < fallbacks.length; i++) {
            try { app.executeCommand(fallbacks[i]); } catch (e2) {}
        }
        return '{"success":true,"method":"fallback","ids":"' + fallbacks.join(',') + '"}';
    } catch (e) {
        return _ae_err('ae_openOMTemplateEditor: ' + e);
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
