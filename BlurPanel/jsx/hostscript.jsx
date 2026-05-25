/**
 * hostscript.jsx — evalScript entry point
 * All functions called from panel JS live here or are #include'd below.
 * ES3 only: no let/const, no arrow functions, no template literals.
 */

//@include "ae.jsx"
//@include "pr.jsx"

/**
 * Returns the host application identifier.
 * "AEFT" = After Effects, "PPRO" = Premiere Pro
 */
function getAppId() {
    var name = app.appName;
    if (name.indexOf("After Effects") !== -1) return "AEFT";
    if (name.indexOf("Premiere") !== -1)       return "PPRO";
    return name;
}
