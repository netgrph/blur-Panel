/**
 * settings.js — User settings persistence via Node.js fs.
 * Reads/writes BlurPanel/config/user-settings.json.
 * Must be loaded after CSInterface.js.
 */

var Settings = (function() {
    var fs   = require('fs');
    var path = require('path');

    var _csInterface  = new CSInterface();
    var _settingsPath = path.join(
        _csInterface.getSystemPath(SystemPath.EXTENSION),
        'config',
        'user-settings.json'
    );

    var DEFAULTS = {
        // Blur
        blur_amount:   1.0,
        blur_weighting: 'equal',
        blur_gamma:    1.0,

        // Interpolation
        interpolate:          true,
        interpolated_fps:     1200,
        interpolation_method: 'svp',
        deduplicate:          true,
        pre_interpolate:      false,
        pre_interpolated_fps: 360,

        // Output
        encode_preset:    'h264',
        quality:          16,
        gpu_decoding:     true,
        gpu_interpolation: true,
        gpu_encoding:     false,

        // Filters
        filters:     false,
        brightness:  1.0,
        saturation:  1.0,
        contrast:    1.0
    };

    function load() {
        try {
            var raw  = fs.readFileSync(_settingsPath, 'utf8');
            var data = JSON.parse(raw);
            // Merge with defaults so new keys always have values
            var merged = {};
            var key;
            for (key in DEFAULTS) {
                if (DEFAULTS.hasOwnProperty(key)) {
                    merged[key] = data.hasOwnProperty(key) ? data[key] : DEFAULTS[key];
                }
            }
            return merged;
        } catch (e) {
            return JSON.parse(JSON.stringify(DEFAULTS));
        }
    }

    function save(obj) {
        try {
            var dir = path.dirname(_settingsPath);
            if (!fs.existsSync(dir)) {
                fs.mkdirSync(dir, { recursive: true });
            }
            fs.writeFileSync(_settingsPath, JSON.stringify(obj, null, 2), 'utf8');
        } catch (e) {
            console.error('BlurPanel: failed to save settings —', e.message);
        }
    }

    function getDefaults() {
        return JSON.parse(JSON.stringify(DEFAULTS));
    }

    return { load: load, save: save, getDefaults: getDefaults };
}());
