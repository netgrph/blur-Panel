# Knowledgebase Reference

Optimized single-file reference for building the blur Ae/Pr **CEP panel**.
Source guides are in `AE-PR PLUGIN & SCRIPTING GUIDES/` — read-only, never edit.

---

## 0. CEP Panel Architecture

The panel is a **CEP (Common Extensibility Platform)** extension, not a ScriptUI panel. This avoids the "Allow Scripts to Write Files and Access Network" permission entirely.

### Two runtime layers

| Layer | Language | Runs in | Role |
|---|---|---|---|
| Panel UI | HTML/CSS/JS + Node.js | Chromium (CEP) | UI, file I/O, process spawning |
| Host bridge | ExtendScript (ES3) | Ae/Pr engine | Ae/Pr API calls only |

### Communication pattern

```javascript
// Panel JS → ExtendScript
csInterface.evalScript('functionName(arg)', function(result) {
    // result is always a string; parse JSON for structured data
    var obj = JSON.parse(result);
});

// ExtendScript always returns a JSON string:
function myFunc() { return JSON.stringify({ success: true }); }
```

### Node.js access

With `--enable-nodejs` + `--mixed-context` in the manifest, `require()` works in the panel JS context:

```javascript
var fs            = require('fs');
var path          = require('path');
var os            = require('os');
var child_process = require('child_process');
```

### App detection

```javascript
var csInterface = new CSInterface();
var appId = csInterface.getHostEnvironment().appId; // 'AEFT' | 'PPRO'
```

### Extension root path

```javascript
var extRoot = csInterface.getSystemPath(SystemPath.EXTENSION);
// On Windows, returns native backslash path (CSInterface.js normalises it)
```

### Installation (development)

Set registry key (all three CSXS versions to cover Ae/Pr 2019–latest):
```
HKCU\Software\Adobe\CSXS.9\PlayerDebugMode  = 1
HKCU\Software\Adobe\CSXS.10\PlayerDebugMode = 1
HKCU\Software\Adobe\CSXS.11\PlayerDebugMode = 1
```
Then place `BlurPanel/` at `%APPDATA%\Adobe\CEP\extensions\com.blur.panel\` and restart the host app.

### ES3 constraint for .jsx files

All ExtendScript files must be ES3:
- No `let` / `const` — use `var`
- No arrow functions — use `function()`
- No template literals — use string concatenation
- No `Array.forEach` — use `for` loops (or check availability first)
- No destructuring, spread, optional chaining

---

## 1. blur-master

### CLI Invocation

```bash
blur-cli -i input.mp4 [-o output.mp4] [-c config.cfg] [-p] [-v]
```

- `-i` — input file(s), required
- `-o` — output file(s); if omitted, auto-generated alongside input
- `-c` — config file path(s); if omitted, reads `.blur-config.cfg` from the video folder
- `-p` — open preview window
- `-v` — verbose output

Multiple files: `-i a.mp4 b.mp4 -o a_out.mp4 b_out.mp4` (counts must match).

### Internal Pipeline

blur-master runs two processes piped together:

```
vspipe -p -c y4m -a video_path=<path> -a fps_num=<n> -a fps_den=<d>
       -a color_range=<pc|tv|undefined> -a settings=<JSON>
       -a enable_lsmash=true  blur.py  -
| ffmpeg -loglevel error -hide_banner -stats -y
         -i -  -fflags +genpts  -i <original>
         -map 0:v  -map 1:a?  ...  output.mp4
```

The script never needs to invoke vspipe/ffmpeg directly — that is blur-master's responsibility.

### Config File (`.blur-config.cfg`)

Written by the script to a temp folder before calling blur-cli via `-c`. Key fields:

#### Core Blur

| Field | Default | Notes |
|---|---|---|
| `blur` | `true` | Enable frame blending |
| `blur_amount` | `1.0` | 0=none, 0.5=medium, 1=full blend, >1 ghosting |
| `blur_output_fps` | `60` | Output FPS — **must match comp/sequence FPS** |
| `blur_weighting` | `"equal"` | `equal`, `gaussian_sym`, `vegas`, `pyramid`, `gaussian`, `ascending`, `descending`, `gaussian_reverse`, or comma-sep custom weights |
| `blur_gamma` | `1.0` | Gamma correction applied during blending |

#### Interpolation

| Field | Default | Notes |
|---|---|---|
| `interpolate` | `true` | Enable frame interpolation |
| `interpolated_fps` | `1200` (Win/Linux) / `600` (macOS) | Target interpolation FPS; ≤10× input FPS recommended |
| `interpolation_method` | `svp` (Win/Linux) / `rife` (macOS) | `svp` or `rife` |
| `pre_interpolate` | `false` | AI pre-interpolation pass first (slower) |
| `pre_interpolated_fps` | `360` | Target FPS for pre-interpolation pass |

#### Deduplication

| Field | Default | Notes |
|---|---|---|
| `deduplicate` | `true` | Remove duplicate frames before interpolation |
| `deduplicate_method` | `svp` (Win/Linux) / `rife` (macOS) | `svp` or `rife` |

#### Timescale (Speed Ramping)

| Field | Default | Notes |
|---|---|---|
| `timescale` | `false` | Enable speed ramping |
| `input_timescale` | `1.0` | Speed of input video |
| `output_timescale` | `1.0` | Speed of output video |
| `output_timescale_audio_pitch` | `false` | Pitch-correct audio when timescale changes |

#### Filters

| Field | Default | Notes |
|---|---|---|
| `filters` | `false` | Enable colour adjustments |
| `brightness` | `1.0` | |
| `saturation` | `1.0` | |
| `contrast` | `1.0` | |

#### Encoding

| Field | Default | Notes |
|---|---|---|
| `encode_preset` | `"h264"` | Codec preset |
| `quality` | `16` | CRF value (0=lossless, 51=worst) |

#### Advanced Section (`[advanced]`)

| Field | Default | Notes |
|---|---|---|
| `video_container` | `"mp4"` | `mp4`, `mkv`, `avi` |
| `svp_interpolation_preset` | `"weak"` | `weak`, `film`, `smooth`, `animation`, `default`, `test` |
| `svp_interpolation_algorithm` | `"13"` | `1`, `2`, `11`, `13`, `21`, `23` |
| `interpolation_blocksize` | `"8"` | `4`, `8`, `16`, `32` |
| `rife_model` | `"rife-v4.26_ensembleFalse"` | RIFE model path |
| `deduplicate_range` | `2` | Frames to search for duplicates |
| `deduplicate_threshold` | `"0.001"` | Movement threshold |
| `ffmpeg_override` | `""` | Custom FFmpeg filter string |
| `debug` | `false` | Show debug output |
| `blur_weighting_gaussian_std_dev` | `1.0` | |
| `blur_weighting_gaussian_mean` | `2.0` | |
| `blur_weighting_gaussian_bound` | `"[0,2]"` | |
| `manual_svp` | `false` | Use raw JSON SVP strings |
| `super_string` | `""` | SVSuper JSON params |
| `vectors_string` | `""` | SVAnalyse JSON params |
| `smooth_string` | `""` | SVSmoothFps JSON params |

### Recommended Blur Amounts (60 fps source)

| Intent | blur_amount |
|---|---|
| Low | 0.2 – 0.3 |
| Medium | 0.5 |
| Normal | 1.0 |
| Maximum / ghosting | > 1.0 |

When changing FPS: `new_blur = old_blur × (new_fps / old_fps)`

---

## 2. After Effects Scripting

ExtendScript is ES3 — no arrow functions, no `let`/`const`, no template literals, no destructuring.

### Critical Permission Constraint

`system.callSystem()` **and** all file-write operations require the user to enable:

> **Edit → Preferences → General → "Allow Scripts To Write Files And Access Network"**

This is the hardest constraint in the project. See Section 4 for workarounds.

### Key Objects

#### `app` — Application

```javascript
app.project          // current project
app.activeViewer     // focused viewer panel
app.disableRendering // pause rendering (boolean)
```

#### `app.project` — Project

```javascript
// Import footage from disk
var footage = app.project.importFile(new ImportOptions(new File("C:\\path\\video.mp4")));

// All items (comps, footage, folders)
app.project.items          // ItemCollection
app.project.renderQueue    // RenderQueue
app.project.file           // File object of .aep (null if unsaved)
```

#### `CompItem` — Composition

```javascript
var comp = app.project.items[i]; // if it's a comp

comp.width; comp.height;
comp.frameRate;          // frames per second (Number)
comp.frameDuration;      // 1 / frameRate
comp.duration;           // seconds
comp.workAreaStart;      // seconds
comp.workAreaDuration;   // seconds
comp.layers;             // LayerCollection
comp.numLayers;
comp.selectedLayers;     // Array

// Create new comp
var c = app.project.items.addComp("Name", 1920, 1080, 1.0, 10.0, 29.97);
```

#### `LayerCollection` — Adding Layers

```javascript
comp.layers.add(footageItem);           // add footage as layer
comp.layers.addSolid([r,g,b], "name", w, h, 1.0);
comp.layers.addNull();
```

#### `RenderQueue` — Rendering

```javascript
var rq = app.project.renderQueue;

// Add comp to queue
var rqItem = rq.items.add(comp);

// Set output path (index 1 = first output module)
rqItem.outputModule(1).file = new File("C:\\output.avi");

// Set render settings template
rqItem.outputModule(1).applyTemplate("Lossless");

// Start render — BLOCKS until complete
rq.render();

// Other controls
rq.rendering;          // boolean
rq.pauseRendering(true);
rq.stopRendering();
rq.showWindow(true);
```

`rq.render()` is synchronous — execution does not continue until rendering finishes.

#### `system` — External Commands

```javascript
// Returns stdout as string; REQUIRES permission
var output = system.callSystem('cmd.exe /c "blur-cli -i in.mp4 -o out.mp4"');
```

#### `File` / `Folder` — File System

```javascript
var f = new File("C:\\path\\file.mp4");
f.exists;
f.path;           // directory portion
f.name;           // filename only
f.fsName;         // OS-native path string

// Read text file (no permission needed for reads)
f.open("r");
var contents = f.read();
f.close();

// Write text file (REQUIRES permission)
f.open("w");
f.write("content");
f.close();

var folder = new Folder("C:\\path");
folder.exists;
folder.create();  // REQUIRES permission
```

#### Settings Persistence — `app.settings`

Does NOT require any special permission. Use this for all user preferences.

```javascript
// Write
app.settings.saveSetting("SectionName", "keyName", "value");

// Read (returns string always)
var val = app.settings.getSetting("SectionName", "keyName");

// Check existence
if (app.settings.haveSetting("SectionName", "keyName")) { ... }
```

All values stored as strings — parse numbers with `parseFloat()` / `parseInt()`.

### ScriptUI Panel Template

```javascript
// Dockable panel (window type auto-detected by host)
var panel = (this instanceof Panel) ? this : new Window("palette", "Blur Panel", undefined, {resizeable: true});

// Build UI
var btn = panel.add("button", undefined, "Apply");
btn.onClick = function() { doApply(); };

// Show (only if not docked)
if (panel instanceof Window) {
    panel.center();
    panel.show();
}
panel.layout.layout(true);
```

Using `(this instanceof Panel)` makes the script work both as a floating Window and as a docked panel.

### Pre-Render Workflow (Ae)

```javascript
function preRenderComp(comp, outputPath) {
    var rq = app.project.renderQueue;
    var rqItem = rq.items.add(comp);
    rqItem.outputModule(1).file = new File(outputPath);
    // Optionally: rqItem.outputModule(1).applyTemplate("Lossless");
    rq.render(); // blocks until done
}
```

### Import Footage Back Into Comp

```javascript
function importAndAddToComp(comp, filePath) {
    var importOpts = new ImportOptions(new File(filePath));
    var footage = app.project.importFile(importOpts);
    var layer = comp.layers.add(footage);
    layer.moveToBeginning(); // place at top (highest layer)
    return layer;
}
```

---

## 3. Premiere Pro Scripting

### Key Objects

#### `app.project`

```javascript
app.project.sequences;      // SequenceCollection — all sequences
app.project.projectItems;   // ProjectItemCollection — bin contents
app.project.documentID;     // unique string ID
```

#### `Sequence` — Timeline

```javascript
var seq = app.project.sequences[0]; // or app.project.activeSequence

seq.name;
seq.frameSizeHorizontal;   // width
seq.frameSizeVertical;     // height
seq.videoTracks;
seq.audioTracks;
seq.end;                   // end time in ticks
seq.zeroPoint;             // start time in ticks

// FPS calculation from ticks
var fps = 254016000000 / seq.timebase;
```

#### `app.encoder` — Export to AME

```javascript
// Encode entire sequence
var jobID = app.encoder.encodeSequence(
    seq,
    "C:\\output\\sequence.mp4",    // output path
    "C:\\preset.epr",              // AME preset path (empty string = use default)
    app.encoder.ENCODE_ENTIRE,     // work area: ENCODE_ENTIRE, ENCODE_IN_TO_OUT, ENCODE_WORK_AREA
    true                           // removeUponCompletion
);
// Returns job ID string, or "0" on failure

app.encoder.startBatch(); // start AME rendering
app.encoder.launchEncoder(); // open AME window
```

**Note:** `encodeSequence` is asynchronous — unlike Ae's `renderQueue.render()`, execution continues immediately. A job ID is returned to poll status if needed.

#### Sequence FPS

```javascript
var fps = 254016000000 / seq.timebase;
```

### Pre-Render Workflow (Pr)

```javascript
function preRenderSequence(seq, outputPath) {
    var jobID = app.encoder.encodeSequence(
        seq, outputPath, "", app.encoder.ENCODE_ENTIRE, true
    );
    if (jobID === "0") {
        alert("Export failed to queue.");
        return false;
    }
    app.encoder.startBatch();
    return jobID;
}
```

### Settings Persistence (Pr)

Premiere Pro does not have `app.settings`. Use a JSON file written to a known location (requires permission), or use a ScriptUI hidden-state workaround. Recommended: write a config JSON to the script's own folder.

---

## 4. Critical Constraint: External Process Without Permission

The "Allow Scripts To Write Files And Access Network" permission gates `system.callSystem()`. The goal is to avoid requiring the user to enable it.

### Strategy: No Permission Required

**Option A — Write a temp .bat/.sh and open via `Folder.execute()` / `File.execute()`**

`File.execute()` opens a file using the OS default handler (like double-clicking it). A `.bat` file opens cmd.exe and runs blur-cli. **Does not require permission.**

```javascript
// Write .bat file to temp dir — this DOES require permission
var bat = new File(Folder.temp + "/run_blur.bat");
bat.open("w");
bat.write('@echo off\r\nblur-cli -i "' + inputPath + '" -o "' + outputPath + '"\r\n');
bat.close();
bat.execute(); // no permission needed to EXECUTE
```

Caveat: Writing the `.bat` itself requires permission. If the script folder is bundled and writeable, write to `$.fileName`'s folder instead.

**Option B — Pre-write the .bat in the distribution package**

Bundle a parameterised launcher script (`run_blur.bat` / `run_blur.sh`) in the distribution folder that reads arguments from a side-car text file. The Ae/Pr script writes that text file (requires permission for file write), or passes args via environment (not possible in ExtendScript).

**Option C — AME Post-Encode Script (Pr only)**

Premiere's AME integration supports post-encode notifications via script. After encoding, trigger blur-cli.

**Option D — Accept the permission and guide the user**

Check at startup whether the permission is granted and show a one-time setup dialog if not. This is the simplest path if the workarounds prove brittle.

```javascript
function checkPermission() {
    try {
        system.callSystem("echo test");
        return true;
    } catch (e) {
        return false;
    }
}
```

**If uncertain which strategy to use, present options to the developer before implementing.**

---

## 5. Cross-Cutting Patterns

### Get Active Comp / Sequence

```javascript
// After Effects
var comp = app.project.activeItem; // null if nothing active, or not a comp
if (!(comp instanceof CompItem)) { alert("Select a composition."); return; }

// Premiere Pro
var seq = app.project.activeSequence; // null if nothing active
if (!seq) { alert("Select a sequence."); return; }
```

### Get FPS from Active Item

```javascript
// After Effects
var fps = comp.frameRate; // direct Number

// Premiere Pro
var fps = 254016000000 / seq.timebase;
```

### Temp File Paths

```javascript
var tempDir  = Folder.temp.fsName;            // OS temp folder
var scriptDir = new File($.fileName).parent.fsName; // script's own folder
```

### Escaping Paths in Shell Commands (Windows)

```javascript
var cmd = 'cmd.exe /c "blur-cli -i "' + inputPath.replace(/\\/g, "\\\\") + '" -o "' + outputPath.replace(/\\/g, "\\\\") + '""';
system.callSystem(cmd);
```

Safer pattern — wrap the whole command in outer double quotes and inner double quotes around paths:

```javascript
var cmd = 'cmd.exe /c ""' + blurCliPath + '" -i "' + inputPath + '" -o "' + outputPath + '""';
```

### Config File Writing

Write a `.blur-config.cfg` to a temp or script-adjacent folder, then pass it with `-c`:

```javascript
var cfg = new File(Folder.temp.fsName + "\\blur_temp.cfg");
cfg.open("w");
cfg.writeln("blur = true");
cfg.writeln("blur_amount = " + blurAmount);
cfg.writeln("blur_output_fps = " + compFps);
cfg.writeln("interpolate = " + interpolate);
cfg.writeln("interpolated_fps = " + interpFps);
// ... etc
cfg.close();
```

---

## 6. Distribution Bundle Structure

```
BlurPanel/
├── BlurPanel.jsx          ← the ScriptUI panel script
├── blur-cli.exe           ← blur-master CLI binary (Windows)
├── blur.py                ← VapourSynth processing script
├── lib/                   ← blur-master support files
│   ├── vapoursynth/
│   └── ffmpeg/
└── run_blur.bat           ← optional launcher script
```

The user drops `BlurPanel/` anywhere and runs BlurPanel.jsx from File → Scripts → Run Script File, or copies to the AE/Pr Scripts folder for menu access.
