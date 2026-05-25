/**
 * CSInterface.js — Adobe CEP bridge (compatible with CEP 9–11)
 * Provides communication between the panel HTML/JS and the ExtendScript host.
 */

var SystemPath = {
    USER_DATA:    "userData",
    COMMON_FILES: "commonFiles",
    MY_DOCUMENTS: "myDocuments",
    APPLICATION:  "application",
    EXTENSION:    "extension",
    HOST_APPLICATION: "hostApplication"
};

var CSXSWindowType = {
    PANEL:   "Panel",
    MODELESS:"Modeless",
    MODAL:   "Modal"
};

function HostEnvironment(appName, appVersion, appLocale, appUILocale, appId, isAppOnline, appSkinInfo) {
    this.appName = appName;
    this.appVersion = appVersion;
    this.appLocale = appLocale;
    this.appUILocale = appUILocale;
    this.appId = appId;
    this.isAppOnline = isAppOnline;
    this.appSkinInfo = appSkinInfo;
}

function AppSkinInfo(baseFontFamily, baseFontSize, imageIconSize, systemHighlightColor, panelBackgroundColor, panelForegroundColor) {
    this.baseFontFamily = baseFontFamily;
    this.baseFontSize = baseFontSize;
    this.imageIconSize = imageIconSize;
    this.systemHighlightColor = systemHighlightColor;
    this.panelBackgroundColor = panelBackgroundColor;
    this.panelForegroundColor = panelForegroundColor;
}

function RGBColor(red, green, blue, alpha) {
    this.red = red;
    this.green = green;
    this.blue = blue;
    this.alpha = alpha;
}

function UIColor(type, antialiasLevel, color) {
    this.type = type;
    this.antialiasLevel = antialiasLevel;
    this.color = color;
}

var CSInterface = (function() {
    function CSInterface() {
        var cepVersion = this.getCurrentApiVersion();
        this.hostEnvironment = this.getHostEnvironment();
    }

    CSInterface.THEME_COLOR_CHANGED_EVENT = "com.adobe.csxs.events.ThemeColorChanged";

    CSInterface.prototype.getHostEnvironment = function() {
        if (typeof window.__adobe_cep__ !== "undefined") {
            var envStr = window.__adobe_cep__.getHostEnvironment();
            var env = JSON.parse(envStr);
            return env;
        }
        return { appName: "", appVersion: "", appId: "", appLocale: "", appUILocale: "" };
    };

    CSInterface.prototype.getCurrentApiVersion = function() {
        if (typeof window.__adobe_cep__ !== "undefined") {
            var ver = JSON.parse(window.__adobe_cep__.getCurrentApiVersion());
            return ver;
        }
        return { major: 9, minor: 0, micro: 0 };
    };

    CSInterface.prototype.getSystemPath = function(pathType) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            var path = window.__adobe_cep__.getSystemPath(pathType);
            // On Windows, CEP returns paths with forward slashes — normalize
            if (path && path.indexOf("file:///") === 0) {
                path = path.replace("file:///", "");
                // Convert forward slashes to backslashes on Windows
                if (navigator.platform.indexOf("Win") !== -1) {
                    path = path.replace(/\//g, "\\");
                }
            }
            return path;
        }
        return "";
    };

    CSInterface.prototype.evalScript = function(script, callback) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            if (callback === null || callback === undefined) {
                callback = function(result) {};
            }
            window.__adobe_cep__.evalScript(script, callback);
        } else if (typeof window.cep !== "undefined" && typeof window.cep.evalScript !== "undefined") {
            window.cep.evalScript(script, callback);
        } else {
            if (callback) callback("EvalScript error: CEP not available");
        }
    };

    CSInterface.prototype.addEventListener = function(type, listener, obj) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            window.__adobe_cep__.addEventListener(type, listener, obj);
        }
    };

    CSInterface.prototype.removeEventListener = function(type, listener, obj) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            window.__adobe_cep__.removeEventListener(type, listener, obj);
        }
    };

    CSInterface.prototype.dispatchEvent = function(event) {
        if (typeof event.data === "object") {
            event.data = JSON.stringify(event.data);
        }
        if (typeof window.__adobe_cep__ !== "undefined") {
            window.__adobe_cep__.dispatchEvent(event);
        }
    };

    CSInterface.prototype.getExtensions = function(extensionIds) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            var idStr = JSON.stringify(extensionIds);
            var str = window.__adobe_cep__.getExtensions(idStr);
            return JSON.parse(str);
        }
        return [];
    };

    CSInterface.prototype.openExtension = function(extensionId, params) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            window.__adobe_cep__.requestOpenExtension(extensionId, params);
        }
    };

    CSInterface.prototype.closeExtension = function() {
        if (typeof window.__adobe_cep__ !== "undefined") {
            window.__adobe_cep__.closeExtension();
        }
    };

    CSInterface.prototype.getScaleFactor = function() {
        if (typeof window.__adobe_cep__ !== "undefined") {
            return window.__adobe_cep__.getScaleFactor();
        }
        return 1;
    };

    CSInterface.prototype.setContextMenu = function(menu, callback) {
        if (typeof window.__adobe_cep__ !== "undefined") {
            window.__adobe_cep__.setContextMenu(menu, callback);
        }
    };

    return CSInterface;
}());
