/**
 * main.js — Panel controller.
 * Handles: app detection, UI ↔ settings binding, config file writing,
 * ExtendScript calls, blur-cli process management, progress display.
 */

// ── Error banner (shown when the module fails to load) ───────────────────────

function _showBanner(msg) {
    var el = document.getElementById('error-banner');
    if (el) { el.textContent = msg; el.style.display = 'block'; }
    console.error('BlurPanel:', msg);
}

if (typeof require !== 'function') {
    _showBanner('Node.js (require) is not available. ' +
        'Check that --enable-nodejs and --mixed-context are in CSXS/manifest.xml.');
}

// ── Main module ───────────────────────────────────────────────────────────────

var App;

try {

App = (function() {

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

    // ffmpeg lives in bin/lib/ffmpeg/ (installer layout) or bin/ffmpeg/ (flat).
    function _findFfmpeg() {
        var exe = process.platform === 'win32' ? 'ffmpeg.exe' : 'ffmpeg';
        var p1  = path.join(_binDir, 'lib', 'ffmpeg', exe);
        var p2  = path.join(_binDir, 'ffmpeg', exe);
        if (fs.existsSync(p1)) return p1;
        if (fs.existsSync(p2)) return p2;
        return null;
    }

    // ── Initialisation ────────────────────────────────────────────────────────

    function init() {
        try {
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
                _settings.interpolation_method = 'rife';
            }

            // Read all JSX files in Node.js and evaluate as one combined string.
            // This avoids #include path-resolution issues entirely.
            var jsxDir  = path.join(_extRoot, 'jsx');
            var aeSrc   = fs.readFileSync(path.join(jsxDir, 'ae.jsx'),         'utf8');
            var prSrc   = fs.readFileSync(path.join(jsxDir, 'pr.jsx'),         'utf8');
            var hostSrc = fs.readFileSync(path.join(jsxDir, 'hostscript.jsx'), 'utf8')
                            .replace(/\/\/@include[^\r\n]*/g, '');
            var allJsx  = aeSrc + '\n' + prSrc + '\n' + hostSrc;
            csInterface.evalScript(allJsx, function(r) {
                if (r && typeof r === 'string' && r.indexOf('Error') !== -1) {
                    _showBanner('ExtendScript load error: ' + r);
                }
            });

            bindUI();
            applySettingsToUI(_settings);
            wireCollapsibles();
            wireHqModal();
            updateHqBadge(_settings.hq_template_status || 'unknown');

            // Hide AE-only UI on Premiere (HQ template + AE re-import format dropdown)
            if (_appId === 'PPRO') {
                var aeOnlyIds = ['hq-template-row', 'hq-divider', 'ae-import-row', 'ae-import-divider'];
                for (var k = 0; k < aeOnlyIds.length; k++) {
                    var el = document.getElementById(aeOnlyIds[k]);
                    if (el) el.style.display = 'none';
                }
            }


        } catch (e) {
            _showBanner('init() error: ' + e.message + ' — ' + e.stack);
        }
    }

    // ── UI binding ────────────────────────────────────────────────────────────

    var SLIDER_PAIRS = [
        ['blur_amount',  'blur_amount_num'],
        ['blur_gamma',   'blur_gamma_num'],
        ['quality',      'quality_num'],
        ['brightness',   'brightness_num'],
        ['saturation',   'saturation_num'],
        ['contrast',     'contrast_num']
    ];

    function bindUI() {
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

        var plain = [
            'blur_weighting', 'interpolate', 'interpolated_fps',
            'deduplicate', 'deduplicate_method',
            'pre_interpolate', 'pre_interpolated_fps',
            'encode_preset', 'gpu_decoding', 'gpu_interpolation', 'gpu_encoding',
            'ae_import_format',
            'filters'
        ];
        plain.forEach(function(id) {
            var el = document.getElementById(id);
            if (!el) return;
            el.addEventListener('change', saveFromUI);
            el.addEventListener('input',  saveFromUI);
        });

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
        setVal('deduplicate_method',   s.deduplicate_method);
        setVal('pre_interpolate',      s.pre_interpolate);
        setVal('pre_interpolated_fps', s.pre_interpolated_fps);

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
        setVal('ae_import_format',  s.ae_import_format);

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
        _settings.deduplicate_method   = getVal('deduplicate_method');
        _settings.pre_interpolate      = getVal('pre_interpolate');
        _settings.pre_interpolated_fps = parseInt(getVal('pre_interpolated_fps'), 10);

        _settings.encode_preset     = getVal('encode_preset');
        _settings.quality           = parseInt(getVal('quality'), 10);
        _settings.gpu_decoding      = getVal('gpu_decoding');
        _settings.gpu_interpolation = getVal('gpu_interpolation');
        _settings.gpu_encoding      = getVal('gpu_encoding');
        _settings.ae_import_format  = getVal('ae_import_format');

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
            '[blur v2.42]',
            '',
            '- blur',
            'blur: true',
            'blur amount: ' + s.blur_amount,
            'blur output fps: ' + compFps,
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
            'deduplicate method: ' + (s.deduplicate_method || s.interpolation_method),
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
            'input timescale: 1',
            'output timescale: 1',
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

    // No module-level regex — _checkProgress creates a fresh /g instance each call
    // so it always picks the LAST N/M in the buffer, not the first (stale) one.

    function setStatus(text) {
        var el = document.getElementById('phase-text');
        if (el) el.textContent = text;
        var bar = document.getElementById('progress-bar');
        if (bar) bar.classList.add('busy');
        console.log('BlurPanel status:', text);
    }

    function setProgress(cur, total) {
        var pct  = total > 0 ? Math.round((cur / total) * 100) : 0;
        var bar  = document.getElementById('progress-bar');
        var fill = document.getElementById('progress-fill');
        var ft   = document.getElementById('frame-text');
        if (bar)  bar.classList.remove('busy');
        if (fill) fill.style.width = pct + '%';
        if (ft)   ft.textContent = 'Frame ' + cur + ' / ' + total + ' · ' + pct + '%';
    }

    function showProgress(show) {
        var wrap  = document.getElementById('progress-wrap');
        var phase = document.getElementById('phase-text');
        var bar   = document.getElementById('progress-bar');
        var fill  = document.getElementById('progress-fill');
        var ft    = document.getElementById('frame-text');
        if (wrap)  wrap.classList.toggle('hidden', !show);
        if (phase) phase.classList.toggle('hidden', !show);
        if (!show) {
            if (bar)  bar.classList.remove('busy');
            if (fill) fill.style.width = '0%';
            if (ft)   ft.textContent = '';
        }
    }

    function setApplyBusy(busy) {
        var btn = document.getElementById('apply-btn');
        if (!btn) return;
        if (busy) {
            btn.textContent = 'Stop';
            btn.onclick = function() { App.stop(); };
        } else {
            btn.textContent = 'Apply';
            btn.onclick = function() { App.apply(); };
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
        setTimeout(function() { showProgress(false); }, 8000);
        console.error('BlurPanel abort:', msg);
    }

    // ── blur-cli spawner ──────────────────────────────────────────────────────

    function runBlur(inputPath, outputPath, cfgPath, onDone) {
        if (!fs.existsSync(_blurCli)) {
            abort('blur-cli not found at: ' + _blurCli);
            return;
        }

        setStatus('Running blur…');

        // Set cwd to bin/ and inject vapoursynth dir into PATH.
        // Support two layouts:
        //   installer layout: blur-cli.exe + lib/vapoursynth/vspipe.exe  (preferred)
        //   flat layout:      blur-cli.exe + vapoursynth/vspipe.exe      (fallback)
        var libDir     = path.join(_binDir, 'lib');
        var vsInstaller = path.join(libDir, 'vapoursynth');   // installer layout
        var vsFlat      = path.join(_binDir, 'vapoursynth');  // flat layout
        var vsDir = fs.existsSync(vsInstaller) ? vsInstaller : vsFlat;
        var spawnEnv = {};
        var k;
        for (k in process.env) { if (process.env.hasOwnProperty(k)) spawnEnv[k] = process.env[k]; }
        spawnEnv['PATH'] = _binDir + ';' + libDir + ';' + vsDir + ';' + (process.env['PATH'] || '');

        _blurProc = child_process.spawn(_blurCli, [
            '-i', inputPath,
            '-o', outputPath,
            '-c', cfgPath
        ], {
            cwd: _binDir,
            env: spawnEnv
        });

        var stderrBuf = '';
        var stdoutBuf = '';

        function _checkProgress(buf) {
            var re = /(\d+)\/(\d+)/g;
            var m, last;
            while ((m = re.exec(buf)) !== null) { last = m; }
            if (last) setProgress(parseInt(last[1], 10), parseInt(last[2], 10));
            // Discard everything up to the last newline so old lines don't
            // pollute the next regex search.
            var nl = buf.lastIndexOf('\n');
            return nl >= 0 ? buf.slice(nl + 1) : buf.slice(-100);
        }

        _blurProc.stdout.on('data', function(chunk) {
            var s = chunk.toString();
            console.log('blur-cli stdout:', s);
            stdoutBuf += s;
            stdoutBuf = _checkProgress(stdoutBuf);
        });

        _blurProc.stderr.on('data', function(chunk) {
            var s = chunk.toString();
            console.log('blur-cli stderr:', s);
            stderrBuf += s;
            stderrBuf = _checkProgress(stderrBuf);
        });

        _blurProc.on('close', function(code) {
            _blurProc = null;
            if (code === 0) {
                onDone(outputPath);
            } else {
                abort('blur-cli exited with code ' + code);
            }
        });

        _blurProc.on('error', function(err) {
            _blurProc = null;
            abort('Failed to start blur-cli: ' + err.message);
        });
    }

    // ── AE re-import transcode ────────────────────────────────────────────────
    // AE's H.264 decoder corrupts on replay when frames depend on a distant
    // keyframe (default GOP=250). Transcoding to all-intra H.264 or ProRes
    // before import sidesteps the issue. blur-cli's output is the source.

    function transcodeForAe(inputPath, baseStem, onDone) {
        var ffmpeg = _findFfmpeg();
        if (!ffmpeg) {
            // No ffmpeg — fall through to direct import. Replay corruption may occur.
            console.warn('BlurPanel: ffmpeg not found; importing blur output directly.');
            onDone(inputPath);
            return;
        }

        var fmt    = _settings.ae_import_format || 'h264_lossless';
        var stamp  = Date.now();
        var outExt, args;
        if (fmt === 'prores') {
            outExt = '.mov';
            args   = ['-y', '-i', inputPath,
                      '-c:v', 'prores_ks', '-profile:v', '3',
                      '-vendor', 'apl0', '-pix_fmt', 'yuv422p10le',
                      '-qscale:v', '9', '-an'];
        } else {
            outExt = '.mp4';
            args   = ['-y', '-i', inputPath,
                      '-c:v', 'libx264', '-preset', 'ultrafast',
                      '-crf', '0', '-g', '1', '-keyint_min', '1', '-an'];
        }
        var outputPath = path.join(_tempDir, baseStem + '_ae_' + stamp + outExt);
        args.push(outputPath);

        setStatus('Re-encoding for AE…');
        var proc = child_process.spawn(ffmpeg, args);
        var stderr = '';
        proc.stderr.on('data', function(b) { stderr += b.toString(); });
        proc.on('close', function(code) {
            if (code === 0 && fs.existsSync(outputPath)) {
                onDone(outputPath);
            } else {
                console.warn('BlurPanel: ffmpeg transcode failed (code ' + code +
                             '). Falling back to direct import. stderr:\n' + stderr.slice(-400));
                onDone(inputPath);
            }
        });
        proc.on('error', function(err) {
            console.warn('BlurPanel: ffmpeg spawn error: ' + err.message +
                         '. Falling back to direct import.');
            onDone(inputPath);
        });
    }

    // ── HQ template status + setup wizard ────────────────────────────────────

    function updateHqBadge(status) {
        var badge = document.getElementById('hq-status-badge');
        if (!badge) return;
        badge.classList.remove('hq-unknown', 'hq-missing', 'hq-present');
        if (status === 'present') {
            badge.textContent = '✓ HQ active';
            badge.classList.add('hq-present');
        } else if (status === 'missing') {
            badge.textContent = '⚠ Using 40 Mbps fallback';
            badge.classList.add('hq-missing');
        } else {
            badge.textContent = 'Not set up';
            badge.classList.add('hq-unknown');
        }
    }

    function setHqStatus(status) {
        _settings.hq_template_status = status;
        Settings.save(_settings);
        updateHqBadge(status);
    }

    function hqTemplatePath() {
        return path.join(_extRoot, 'templates', 'Blur Panel HQ.aom');
    }

    function copyToClipboard(text) {
        // Windows: pipe text to `clip`. macOS: `pbcopy`. Linux: `xclip`/`xsel`.
        // We're shipping AE/Pr on Windows + macOS, so handle both.
        try {
            var cmd, args;
            if (process.platform === 'win32') {
                cmd = 'clip';
                args = [];
            } else if (process.platform === 'darwin') {
                cmd = 'pbcopy';
                args = [];
            } else {
                cmd = 'xclip';
                args = ['-selection', 'clipboard'];
            }
            var p = child_process.spawn(cmd, args, { windowsHide: true });
            p.stdin.write(text);
            p.stdin.end();
            return true;
        } catch (e) {
            console.error('BlurPanel: clipboard copy failed —', e.message);
            return false;
        }
    }

    function openHqModal() {
        var modal = document.getElementById('hq-modal');
        if (modal) modal.classList.remove('hidden');
        var s = document.getElementById('hq-modal-status');
        if (s) { s.textContent = ''; s.className = 'hq-modal-status'; }
        var pathEl = document.getElementById('hq-path-display');
        var tpl = hqTemplatePath();
        if (pathEl) pathEl.textContent = tpl;
        if (copyToClipboard(tpl)) {
            setModalStatus('Path copied to clipboard. Ready to paste in AE\'s file dialog.', 'ok');
        }
    }

    function closeHqModal() {
        var modal = document.getElementById('hq-modal');
        if (modal) modal.classList.add('hidden');
    }

    function setModalStatus(text, kind) {
        var s = document.getElementById('hq-modal-status');
        if (!s) return;
        s.textContent = text || '';
        s.className = 'hq-modal-status' + (kind ? ' ' + kind : '');
    }

    function verifyHqTemplate() {
        setModalStatus('Checking…', '');
        csInterface.evalScript('ae_hasHqTemplate()', function(res) {
            var r;
            try { r = JSON.parse(res); } catch (e) {
                setModalStatus('Probe failed: ' + res, 'err');
                return;
            }
            if (r.error) { setModalStatus(r.error, 'err'); return; }
            if (r.present) {
                setHqStatus('present');
                setModalStatus('✓ "Blur Panel HQ" template found. You\'re set!', 'ok');
                setTimeout(closeHqModal, 1500);
            } else {
                setHqStatus('missing');
                var reason = r.reason === 'no comp'
                    ? 'No composition open in AE — open or create a comp, then click Verify again.'
                    : 'Template not found yet. Make sure you named it exactly "Blur Panel HQ" and clicked OK.';
                setModalStatus(reason, 'err');
            }
        });
    }

    function wireHqModal() {
        var setupBtn = document.getElementById('hq-setup-btn');
        if (setupBtn) setupBtn.addEventListener('click', openHqModal);

        var closeBtn = document.getElementById('hq-modal-close');
        if (closeBtn) closeBtn.addEventListener('click', closeHqModal);

        var openEditor = document.getElementById('hq-open-editor');
        if (openEditor) openEditor.addEventListener('click', function() {
            // Re-copy in case the user copied something else after the modal opened
            copyToClipboard(hqTemplatePath());
            setModalStatus('Opening Output Module Templates dialog in AE…', 'ok');
            csInterface.evalScript('ae_openOMTemplateEditor()', function(res) {
                var r;
                try { r = JSON.parse(res); } catch (e) {
                    setModalStatus('Could not open editor. Open it manually in AE: Edit → Templates → Output Module… (path is on your clipboard).', 'err');
                    return;
                }
                if (r.error) {
                    setModalStatus('Could not open editor: ' + r.error + ' — open manually via Edit → Templates → Output Module…', 'err');
                } else {
                    setModalStatus('Switch to AE — if the dialog isn\'t visible, open it via Edit → Templates → Output Module… Then click Load… and press Ctrl+V → Enter → OK.', 'ok');
                }
            });
        });

        var copyBtn = document.getElementById('hq-copy-path');
        if (copyBtn) copyBtn.addEventListener('click', function() {
            if (copyToClipboard(hqTemplatePath())) {
                setModalStatus('Path copied to clipboard.', 'ok');
            } else {
                setModalStatus('Couldn\'t access clipboard — select the path above and copy manually.', 'err');
            }
        });

        var verifyBtn = document.getElementById('hq-verify');
        if (verifyBtn) verifyBtn.addEventListener('click', verifyHqTemplate);

        // Dismiss modal by clicking the backdrop
        var modal = document.getElementById('hq-modal');
        if (modal) modal.addEventListener('click', function(ev) {
            if (ev.target === modal) closeHqModal();
        });
    }

    // ── After Effects flow ────────────────────────────────────────────────────

    function runAe() {
        setStatus('Reading composition…');
        csInterface.evalScript('ae_getActiveComp()', function(result) {
            var info;
            try { info = JSON.parse(result); } catch (e) {
                abort('ae_getActiveComp returned: ' + result);
                return;
            }
            if (info.error) { abort(info.error); return; }

            var baseName    = info.name.replace(/[<>:"/\\|?*]/g, '_');
            var _stamp      = Date.now();
            var mp4RenderOut = path.join(_tempDir, baseName + '_prerender_' + _stamp + '.mp4');
            var blurOut     = path.join(_tempDir, baseName + '_blur_' + _stamp + '.mp4');

            setStatus('Pre-rendering "' + info.name + '"…');

            csInterface.evalScript('ae_preRender(' + JSON.stringify(mp4RenderOut) + ',false)', function(res) {
                var r;
                try { r = JSON.parse(res); } catch (e) { abort('Pre-render response: ' + res); return; }
                if (r.error) { abort(r.error); return; }

                console.log('BlurPanel pre-render: template=' + r.tpl
                          + ' hasHqTemplate=' + r.hasHqTemplate);
                setHqStatus(r.hasHqTemplate ? 'present' : 'missing');

                var mp4Input = (r.actualPath || mp4RenderOut).split('/').join(path.sep);

                var cfgPath = writeCfg(info.fps);
                runBlur(mp4Input, blurOut, cfgPath, function(blurredPath) {
                    if (!fs.existsSync(blurredPath)) {
                        abort('Blur output not found: ' + blurredPath);
                        return;
                    }
                    transcodeForAe(blurredPath, baseName, function(importPath) {
                        setStatus('Importing result…');
                        csInterface.evalScript('ae_importAndAddLayer(' + JSON.stringify(importPath) + ')', function(ir) {
                            var irObj;
                            try { irObj = JSON.parse(ir); } catch (e) { abort('Import response: ' + ir); return; }
                            if (irObj.error) { abort(irObj.error); return; }
                            finish('Done — blurred layer added.');
                            try { fs.unlinkSync(mp4RenderOut); } catch (e) {}
                            // If we transcoded, delete blur-cli's intermediate output.
                            if (importPath !== blurredPath) {
                                try { fs.unlinkSync(blurredPath); } catch (e) {}
                            }
                        });
                    });
                });
            });
        });
    }

    // ── Premiere Pro flow ─────────────────────────────────────────────────────

    function runPr() {
        setStatus('Reading sequence…');
        csInterface.evalScript('pr_getActiveSequence()', function(result) {
            var info;
            try { info = JSON.parse(result); } catch (e) {
                abort('pr_getActiveSequence returned: ' + result);
                return;
            }
            if (info.error) { abort(info.error); return; }

            var baseName  = info.name.replace(/[<>:"/\\|?*]/g, '_');
            var exportOut = path.join(_tempDir, baseName + '_prerender.mp4');
            var blurOut   = path.join(_tempDir, baseName + '_blur.mp4');

            // Pr's exportAsMediaDirect requires a real .epr preset — empty string is rejected.
            // We ship one in the extension bundle so no per-user setup is needed.
            var prPresetPath = path.join(_extRoot, 'templates', 'Blur Panel Pr HQ.epr');
            if (!fs.existsSync(prPresetPath)) {
                abort('Missing Premiere export preset: ' + prPresetPath +
                      ' — create one via File → Export → Media (Ctrl+M), Save Preset, ' +
                      'and place it at templates/Blur Panel Pr HQ.epr in the extension folder.');
                return;
            }

            setStatus('Exporting "' + info.name + '" from Premiere…');

            // exportAsMediaDirect is synchronous: it blocks until the file is written,
            // then returns true/false. No polling, no AME dependency.
            csInterface.evalScript('pr_exportSequence(' + JSON.stringify(exportOut) + ',' + JSON.stringify(prPresetPath) + ')', function(res) {
                var r;
                try { r = JSON.parse(res); } catch (e) { abort('Export response: ' + res); return; }
                if (r.error) { abort(r.error); return; }

                var actualPath = r.outputPath || exportOut;
                if (!fs.existsSync(actualPath)) {
                    abort('Export reported success but output file is missing: ' + actualPath);
                    return;
                }

                var cfgPath = writeCfg(info.fps);
                runBlur(actualPath, blurOut, cfgPath, function(blurredPath) {
                    if (!fs.existsSync(blurredPath)) {
                        abort('Blur output not found: ' + blurredPath);
                        return;
                    }
                    setStatus('Importing result…');
                    csInterface.evalScript('pr_importToSequence(' + JSON.stringify(blurredPath) + ')', function(ir) {
                        var irObj;
                        try { irObj = JSON.parse(ir); } catch (e) { abort('Import response: ' + ir); return; }
                        if (irObj.error) { abort(irObj.error); return; }
                        finish('Done — blurred clip added.');
                        try { fs.unlinkSync(actualPath); } catch (e) {}
                    });
                });
            });
        });
    }

    // ── Public API ────────────────────────────────────────────────────────────

    function apply() {
        if (_running) return;
        _running = true;
        setApplyBusy(true);
        showProgress(true);
        setStatus('Starting…');

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

} catch (e) {
    _showBanner('Panel failed to load: ' + e.message);
    App = {
        apply: function() { _showBanner('Panel error — cannot run. See error above.'); },
        stop:  function() {}
    };
}
