Blur Panel — bin/ folder
========================

This folder must contain the blur-master binaries before the panel can process video.

EASIEST: Run install.bat from the root of this package.
It will automatically detect a blur-master installation and copy the files here.


MANUAL SETUP
------------
1. Download and install blur-master from:
   https://github.com/f0e/blur/releases/latest

2. Copy these files/folders from your blur-master install into THIS folder:

   From %ProgramFiles%\blur\         (or wherever blur is installed)
   ─────────────────────────────────
   blur-cli.exe        ->  bin\blur-cli.exe
   lib\blur.py         ->  bin\blur.py
   lib\ffmpeg\         ->  bin\ffmpeg\
   lib\vapoursynth\    ->  bin\vapoursynth\

   If your blur install has these files at the root (older versions):
   blur.py, ffmpeg\, vapoursynth\ — copy directly here.

The final bin\ layout should look like:
   bin\
   ├── blur-cli.exe
   ├── blur.py
   ├── ffmpeg\
   │   └── ffmpeg.exe
   └── vapoursynth\
       └── (VapourSynth DLLs and plugins)
