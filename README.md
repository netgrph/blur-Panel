# Blur Panel

A dockable CEP panel for **Adobe After Effects** and **Adobe Premiere Pro** that wraps the [f0e/blur](https://github.com/f0e/blur) motion blur / frame interpolation tool.

Configure your blur and interpolation settings in the panel, hit **Apply**, and the panel pre-renders the active composition or sequence, runs it through blur-master with your settings, and imports the processed result back as the top layer.

> The panel is a UI front-end for f0e/blur — all blur and interpolation work is done by f0e/blur itself.

<p align="center">
  <img src="Komp%201_2026-05-28_03-51.gif" alt="Install + usage demo" width="720">
</p>

---

## Compatibility

- **After Effects:** 2019 (16.0) through latest
- **Premiere Pro:** 2019 (13.0) through latest
- **OS:** Windows (full feature set). macOS works but VapourSynth / SVP interpolation is Windows-only — Rife interpolation is forced on macOS.

---

## Install

1. **Download or clone** this repository.
2. **Run `install.bat`** (Windows). It will:
   - Enable unsigned CEP extensions (`PlayerDebugMode` registry keys for CSXS 9 / 10 / 11)
   - Copy `com.blur.panel/` into `%APPDATA%\Adobe\CEP\extensions\`
   - Unpack the bundled ffmpeg binaries from the shipped `.7z` archives
   - Clean up any obsolete legacy startup scripts from previous installer versions
3. **Restart** After Effects or Premiere Pro.
4. Open the panel from **Window → Extensions → Blur Panel**.

Saved user settings in `com.blur.panel/config/user-settings.json` are preserved on re-install.

### Manual install

If `install.bat` cannot be run:

1. Double-click `_enable_debug_mode.reg` to set the registry keys.
2. Copy the `com.blur.panel/` folder into `%APPDATA%\Adobe\CEP\extensions\`.
3. Manually extract `com.blur.panel\bin\ffmpeg\ffmpeg.7z` and `com.blur.panel\bin\lib\ffmpeg\ffmpeg.7z` in place (use 7-Zip — a copy of `7z.exe` is bundled in `com.blur.panel\bin\vapoursynth\`).
4. Restart your Adobe app.

---

## Usage

1. Open a composition (AE) or sequence (Pr).
2. Open **Window → Extensions → Blur Panel**.
3. Adjust the settings under the Blur, Interpolation, Output, and Filters sections.
4. Click **Apply**.

The panel will:
1. Pre-render the active comp / sequence
2. Run blur-master against the pre-rendered file with your chosen settings
3. Import the processed output back into the comp / sequence as the top layer

All settings persist between sessions.

### After Effects: high-bitrate output

By default AE pre-renders at ~40 Mbps using its stock H.264 template. For higher-quality intermediates, use the panel's **Setup HQ template** button — it opens AE's Output Module Templates dialog and copies the bundled `Blur Panel HQ.aom` path to your clipboard. Press **Load…**, paste, **Enter**, then **OK**. The Output card will show a green **HQ active** badge once the template is detected.

### Premiere Pro

Pr exports run directly from Premiere using a bundled high-bitrate H.264 preset.

---

## What's included

```
com.blur.panel/             ← the CEP panel (drops into Adobe's CEP folder)
├── CSXS/manifest.xml
├── index.html
├── css/, js/, jsx/         ← UI, host bridge, ExtendScript
├── templates/              ← bundled "Blur Panel HQ.aom" template
├── config/                 ← per-user settings (written at runtime)
└── bin/                    ← blur-cli.exe + blur.py + ffmpeg + vapoursynth
install.bat                 ← one-step installer (Windows)
_enable_debug_mode.reg      ← manual registry helper
```

The `bin/` folder ships every blur-master dependency required to run, so the panel is self-contained — no extra installs needed.

> **Note:** `ffmpeg.exe` and `ffprobe.exe` are shipped packed inside `ffmpeg.7z` archives (they exceed GitHub's 100 MB per-file limit). `install.bat` unpacks them automatically at install time.

---

## Troubleshooting

- **Panel doesn't appear under Window → Extensions:** Make sure you restarted the host app after install. If the menu entry is missing entirely, the `PlayerDebugMode` registry keys were not applied — re-run `install.bat` or double-click `_enable_debug_mode.reg`.
- **`install.bat` reports it can't copy files:** Close After Effects and Premiere Pro and try again — they lock files in the CEP folder while running.
- **"Using 40 Mbps fallback" badge in the Output card (AE):** The `Blur Panel HQ` output module template hasn't been loaded yet. Use the **Setup HQ template** button.

---

## Credits

- Built on top of [blur-master](https://github.com/f0e/blur) by f0e. All credit for the underlying motion blur and interpolation work belongs to that project.
- Uses Adobe's CEP (Common Extensibility Platform).

---

## License

See [f0e/blur](https://github.com/f0e/blur) for the licensing terms of the bundled blur binaries. The panel code itself is provided as-is.

---

## Disclaimer

Almost this entire project was vibecoded with AI assistance. I do not take credit in any way for creating it.
