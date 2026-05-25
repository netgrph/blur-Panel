/**
 * main.js — Panel controller.
 * Handles: app detection, UI ↔ settings binding, config file writing,
 * ExtendScript calls, blur-cli process management, progress display.
 */

var App = (function() {

    // ── Node.js modules ───────────────────────────────────────────────────────
    var fs            = require('fs');
    var path          = require('path');
    var os            = require('os');
    var child_process = require('child_process');

    // ── CEP ───────────────────────────────────────────────────────────────────
    var csInterface = new CSInterface();

    // ── State ─────────────────────────────────────────────────────────────────
    var _appId       = '';   // 'AEFT' or 'PPRO'
    var _settings    = {};
    var _blurProc    = null; // active blur-cli child process
    var _pollTimer   = null; // Pr export poll interval
    var _running     = false;

    // ── Paths ─────────────────────────────────────────────────────────────────
    var _extRoot  = csInterface.getSystemPath(SystemPath.EXTENSION);
    var _binDir   = path.join(_extRoot, 'bin');
    var _blurCli  = path.join(_binDir, process.platform === 'win32' ? 'blur-cli.exe' : 'blur-cli');
    var _jsxPath  = path.join(_extRoot, 'jsx', 'hostscript.jsx');
    var _tempDir  = os.tmpdir();

    // ── Initialisation ────────────────────────────────────────────────────────

    function init() {
        _settings = Settings.load();

        // Detect host app from CEP environment
        var env = csInterface.getHostEnvironment();
        _appId  = env.appId || 'AEFT';

        // Adapt UI label
        document.getElementById('host-label').textContent =
            _appId === 'PPRO' ? 'Premiere Pro' : 'After Effects';

        // Hide SVP on macOS (not supported)
        if (process.platform === 'darwin') {
            var methodRow = document.getElementById('method-row');
            if (methodRow) methodRow.style.display = 'none';
            // Force RIFE on macOS
            _settings.interpolation_method = 'rife';
        }

        // Load ExtendScript via $.evalFile() so #include directives resolve
        // relative to the jsx/ folder. Backslashes must be forward-slashed.
        var jsxFwd = _jsxPath.split('\\').join('/');
        csInterface.evalScript('$.evalFile("' + jsxFwd + '")', function() {});

        bindUI();
        applySettingsToUI(_settings);
        wireCollapsibles();
    }

    // ── UI binding ────────────────────────────────────────────────────────────

    // Fields that have a paired slider + number input
    var SLIDER_PAIRS = [
        ['blur_amount',  'blur_amount_num'],
        ['blur_gamma',   'blur_gamma_num'],
        ['quality',      'quality_num'],
        ['brightness',   'brightness_num'],
        ['saturation',   'saturation_num'],
        ['contrast',     'contrast_num']
    ];

    function bindUI() {
        // Sync slider ↔ number for paired controls
        SLIDER_PAIRS.forEach(function(pair) {
            var slider = document.getElementById(pair[0]);
            var num    = document.getElementById(pair[1]);
            if (!slider || !num) return;

            slider.addEventListener('input', function() {
                num.value = parseFloat(slider.value);
                saveFromUI();
            });
            num.addEventListener('input', function() {
                var v = parseFloat(num.value);
                if (!isNaN(v)) { slider.value = v; }
                saveFromUI();
            });
        });

        // Plain controls (select, checkbox, number without slider pair)
        var plain = [
            'blur_weighting', 'interpolate', 'interpolated_fps',
            'deduplicate', 'pre_interpolate', 'pre_interpolated_fps',
            'encode_preset', 'gpu_decoding', 'gpu_interpolation', 'gpu_encoding',
            'filters'
        ];
        plain.forEach(function(id) {
            var el = document.getElementById(id);
            if (!el) return;
            el.addEventListener('change', saveFromUI);
            el.addEventListener('input',  saveFromUI);
        });

        // Radio group for interpolation method
        var radios = document.querySelectorAll('input[name="interp_method"]');
        for (var i = 0; i < radios.length; i++) {
            radios[i].addEventListener('change', saveFromUI);
        }
    }

    function applySettingsToUI(s) {
        function setVal(id, val) {
            var el = document.getElementById(id);
            if (!el) return;
            if (el.type === 'checkbox') { el.checked = !!val; }
            else { el.value = val; }
        }

        setVal('blur_amount',        s.blur_amount);
        setVal('blur_amount_num',    s.blur_amount);
        setVal('blur_weighting',     s.blur_weighting);
        setVal('blur_gamma',         s.blur_gamma);
        setVal('blur_gamma_num',     s.blur_gamma);

        setVal('interpolate',          s.interpolate);
        setVal('interpolated_fps',     s.interpolated_fps);
        setVal('deduplicate',          s.deduplicate);
        setVal('pre_interpolate',      s.pre_interpolate);
        setVal('pre_interpolated_fps', s.pre_interpolated_fps);

        // Radio: interpolation_method
        var radios = document.querySelectorAll('input[name="interp_method"]');
        for (var i = 0; i < radios.length; i++) {
            radios[i].checked = (radios[i].value === s.interpolation_method);
        }

        setVal('encode_preset',     s.encode_preset);
        setVal('quality',           s.quality);
        setVal('quality_num',       s.quality);
        setVal('gpu_decoding',      s.gpu_decoding);
        setVal('gpu_interpolation', s.gpu_interpolation);
        setVal('gpu_encoding',      s.gpu_encoding);

        setVal('filters',       s.filters);
        setVal('brightness',    s.brightness);
        setVal('brightness_num',s.brightness);
        setVal('saturation',    s.saturation);
        setVal('saturation_num',s.saturation);
        setVal('contrast',      s.contrast);
        setVal('contrast_num',  s.contrast);
    }

    function saveFromUI() {
        function getVal(id) {
            var el = document.getElementById(id);
            if (!el) return null;
            if (el.type === 'checkbox') return el.checked;
            return el.value;
        }
        function getRadio(name) {
            var radios = document.querySelectorAll('input[name="' + name + '"]');
            for (var i = 0; i < radios.length; i++) {
                if (radios[i].checked) return radios[i].value;
            }
            return null;
        }

        _settings.blur_amount   = parseFloat(getVal('blur_amount'));
        _settings.blur_weighting = getVal('blur_weighting');
        _settings.blur_gamma    = parseFloat(getVal('blur_gamma'));

        _settings.interpolate          = getVal('interpolate');
        _settings.interpolated_fps     = parseInt(getVal('interpolated_fps'), 10);
        _settings.interpolation_method = getRadio('interp_method') || _settings.interpolation_method;
        _settings.deduplicate          = getVal('deduplicate');
        _settings.pre_interpolate      = getVal('pre_interpolate');
        _settings.pre_interpolated_fps = parseInt(getVal('pre_interpolated_fps'), 10);

        _settings.encode_preset     = getVal('encode_preset');
        _settings.quality           = parseInt(getVal('quality'), 10);
        _settings.gpu_decoding      = getVal('gpu_decoding');
        _settings.gpu_interpolation = getVal('gpu_interpolation');
        _settings.gpu_encoding      = getVal('gpu_encoding');

        _settings.filters    = getVal('filters');
        _settings.brightness = parseFloat(getVal('brightness'));
        _settings.saturation = parseFloat(getVal('saturation'));
        _settings.contrast   = parseFloat(getVal('contrast'));

        Settings.save(_settings);
    }

    function wireCollapsibles() {
        var headers = document.querySelectorAll('.card-header');
        for (var i = 0; i < headers.length; i++) {
            headers[i].addEventListener('click', function() {
                var targetId = this.getAttribute('data-toggle');
                var body     = document.getElementById(targetId);
                var arrowId  = 'arrow-' + targetId.replace('-body', '');
                var arrow    = document.getElementById(arrowId);
                if (!body) return;
                body.classList.toggle('hidden');
                if (arrow) arrow.textContent = body.classList.contains('hidden') ? '▶' : '▼';
            });
        }
    }

    // ── Config file writer ────────────────────────────────────────────────────

    function writeCfg(compFps) {
        var s   = _settings;
        var cfgPath = path.join(_tempDir, 'blur_panel.cfg');

        var bool = function(v) { return v ? 'true' : 'false'; };

        var lines = [
            '[blur v0.0.0]',
            '',
            '- blur',
            'blur: ' + bool(true),
            'blur amount: ' + s.blur_amount,
            'blur output fps: ' + compFps,       // always = comp/sequence FPS
            'blur weighting: ' + s.blur_weighting,
            'blur gamma: ' + s.blur_gamma,
            '',
            '- interpolation',
            'interpolate: ' + bool(s.interpolate),
            'interpolated fps: ' + s.interpolated_fps,
            'interpolation method: ' + s.interpolation_method,
            '',
            '- pre-interpolation',
            'pre-interpolate: ' + bool(s.pre_interpolate),
            'pre-interpolated fps: ' + s.pre_interpolated_fps,
            '',
            '- deduplication',
            'deduplicate: ' + bool(s.deduplicate),
            'deduplicate method: ' + s.interpolation_method,
            '',
            '- rendering',
            'encode preset: ' + s.encode_preset,
            'quality: ' + s.quality,
            'preview: false',
            'detailed filenames: false',
            'copy dates: false',
            '',
            '- gpu acceleration',
            'gpu decoding: ' + bool(s.gpu_decoding),
            'gpu interpolation: ' + bool(s.gpu_interpolation),
            'gpu encoding: ' + bool(s.gpu_encoding),
            '',
            '- timescale',
            'timescale: false',
            'input timescale: 1.0',
            'output timescale: 1.0',
            'adjust timescaled audio pitch: false',
            '',
            '- filters',
            'filters: ' + bool(s.filters),
            'brightness: ' + s.brightness,
            'saturation: ' + s.saturation,
            'contrast: ' + s.contrast
        ];

        fs.writeFileSync(cfgPath, lines.join('\n'), 'utf8');
        return cfgPath;
    }

    // ── Progress / status helpers ─────────────────────────────────────────────

    var _progressRe = /Frame:\s*(\d+)\/(\d+)/;

    function setStatus(text) {
        var el = document.getElementById('status-text');
        if (el) el.textContent = text;
    }

    function setProgress(cur, total) {
        var pct = total > 0 ? Math.round((cur / total) * 100) : 0;
        var fill = document.getElementById('progress-fill');
        if (fill) fill.style.width = pct + '%';
        setStatus('Frame ' + cur + ' / ' + total + ' (' + pct + '%)');
    }

    function showProgress(show) {
        var wrap = document.getElementById('progress-wrap');
        if (wrap) {
            wrap.classList.toggle('hidden', !show);
        }
    }

    function setApplyBusy(busy) {
        var btn = document.getElementById('apply-btn');
        if (!btn) return;
        if (busy) {
            btn.textContent = 'Stop';
            btn.onclick = App.stop;
        } else {
            btn.textContent = 'Apply';
            btn.onclick = App.apply;
        }
    }

    function finish(msg) {
        _running = false;
        setApplyBusy(false);
        setStatus(msg || 'Done.');
        setTimeout(function() { showProgress(false); }, 3000);
    }

    function abort(msg) {
        if (_pollTimer)  { clearInterval(_pollTimer); _pollTimer = null; }
        if (_blurProc)   { try { _blurProc.kill(); } catch (e) {} _blurProc = null; }
        _running = false;
        setApplyBusy(false);
        setStatus('Error: ' + (msg || 'unknown'));
        setTimeout(function() { showProgress(false); }, 5000);
        console.error('BlurPanel:', msg);
    }

    // ── blur-cli spawner ──────────────────────────────────────────────────────

    function runBlur(inputPath, outputPath, cfgPath, onDone) {
        if (!fs.existsSync(_blurCli)) {
            abort('blur-cli not found at ' + _blurCli + '. Check the bin/ folder in the extension.');
            return;
        }

        setStatus('Running blur…');
        setProgress(0, 1);

        _blurProc = child_process.spawn(_blurCli, [
            '-i', inputPath,
            '-o', outputPath,
            '-c', cfgPath
        ]);

        var stderrBuf = '';

        _blurProc.stderr.on('data', function(chunk) {
            stderrBuf += chunk.toString();
            var m = _progressRe.exec(stderrBuf);
            if (m) {
                setProgress(parseInt(m[1], 10), parseInt(m[2], 10));
                // Keep only the tail to avoid unbounded buffer growth
                stderrBuf = stderrBuf.slice(-256);
            }
        });

        _blurProc.on('close', function(code) {
            _blurProc = null;
            if (code === 0) {
                onDone(outputPath);
            } else {
                abort('blur-cli exited with code ' + code + '. Check that blur-master dependencies are installed.');
            }
        });

        _blurProc.on('error', function(err) {
            _blurProc = null;
            abort('Failed to start blur-cli: ' + err.message);
        });
    }

    // ── After Effects flow ────────────────────────────────────────────────────

    function runAe() {
        csInterface.evalScript('ae_getActiveComp()', function(result) {
            var info;
            try { info = JSON.parse(result); } catch (e) { abort('Could not read composition info.'); return; }
            if (info.error) { abort(info.error); return; }

            var baseName    = info.name.replace(/[<>:"/\\|?*]/g, '_');
            var preRenderOut = path.join(_tempDir, baseName + '_prerender.avi');
            var blurOut      = path.join(_tempDir, baseName + '_blur.mp4');

            setStatus('Pre-rendering "' + info.name + '"…');

            csInterface.evalScript('ae_preRender(' + JSON.stringify(preRenderOut) + ')', function(res) {
                var r;
                try { r = JSON.parse(res); } catch (e) { abort('Pre-render parse error.'); return; }
                if (r.error) { abort(r.error); return; }

                var cfgPath = writeCfg(info.fps);
                runBlur(preRenderOut, blurOut, cfgPath, function(blurredPath) {
                    setStatus('Importing result…');
                    csInterface.evalScript('ae_importAndAddLayer(' + JSON.stringify(blurredPath) + ')', function(ir) {
                        var irObj;
                        try { irObj = JSON.parse(ir); } catch (e) { abort('Import parse error.'); return; }
                        if (irObj.error) { abort(irObj.error); return; }
                        finish('Done — blurred layer added.');
                        // Clean up temp pre-render
                        try { fs.unlinkSync(preRenderOut); } catch (e) {}
                    });
                });
            });
        });
    }

    // ── Premiere Pro flow ─────────────────────────────────────────────────────

    function runPr() {
        csInterface.evalScript('pr_getActiveSequence()', function(result) {
            var info;
            try { info = JSON.parse(result); } catch (e) { abort('Could not read sequence info.'); return; }
            if (info.error) { abort(info.error); return; }

            var baseName  = info.name.replace(/[<>:"/\\|?*]/g, '_');
            var exportOut = path.join(_tempDir, baseName + '_prerender.mp4');
            var blurOut   = path.join(_tempDir, baseName + '_blur.mp4');

            setStatus('Exporting "' + info.name + '" via AME…');

            csInterface.evalScript('pr_exportSequence(' + JSON.stringify(exportOut) + ')', function(res) {
                var r;
                try { r = JSON.parse(res); } catch (e) { abort('Export queue error.'); return; }
                if (r.error) { abort(r.error); return; }

                // AME is async — poll for output file
                var elapsed = 0;
                var timeout = 3600000; // 1 hour max
                _pollTimer = setInterval(function() {
                    elapsed += 2000;
                    if (fs.existsSync(exportOut)) {
                        clearInterval(_pollTimer);
                        _pollTimer = null;

                        var cfgPath = writeCfg(info.fps);
                        runBlur(exportOut, blurOut, cfgPath, function(blurredPath) {
                            setStatus('Importing result…');
                            csInterface.evalScript('pr_importToSequence(' + JSON.stringify(blurredPath) + ')', function(ir) {
                                var irObj;
                                try { irObj = JSON.parse(ir); } catch (e) { abort('Import parse error.'); return; }
                                if (irObj.error) { abort(irObj.error); return; }
                                finish('Done — blurred clip added.');
                                try { fs.unlinkSync(exportOut); } catch (e) {}
                            });
                        });
                    } else if (elapsed >= timeout) {
                        clearInterval(_pollTimer);
                        _pollTimer = null;
                        abort('AME export timed out after 1 hour.');
                    }
                }, 2000);
            });
        });
    }

    // ── Public API ────────────────────────────────────────────────────────────

    function apply() {
        if (_running) return;
        _running = true;
        setApplyBusy(true);
        showProgress(true);
        setProgress(0, 1);

        if (_appId === 'PPRO') {
            runPr();
        } else {
            runAe();
        }
    }

    function stop() {
        abort('Stopped by user.');
    }

    // ── Boot ──────────────────────────────────────────────────────────────────

    document.addEventListener('DOMContentLoaded', init);

    return { apply: apply, stop: stop };

}());
