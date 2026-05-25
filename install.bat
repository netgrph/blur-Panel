@echo off
setlocal EnableDelayedExpansion
title Blur Panel Installer

echo.
echo  ===========================================
echo   Blur Panel - Installer
echo  ===========================================
echo.

:: ── Paths ────────────────────────────────────────────────────────────────────

set "SCRIPT_DIR=%~dp0"
set "PANEL_SRC=%SCRIPT_DIR%com.blur.panel"
set "CEP_DIR=%APPDATA%\Adobe\CEP\extensions"
set "INSTALL_DIR=%CEP_DIR%\com.blur.panel"

:: ── 1. Verify panel source exists ────────────────────────────────────────────

if not exist "%PANEL_SRC%\CSXS\manifest.xml" (
    echo  [ERROR] Could not find com.blur.panel\CSXS\manifest.xml
    echo          Make sure install.bat is in the same folder as com.blur.panel\
    echo.
    pause & exit /b 1
)

:: ── 2. Enable unsigned CEP extensions (PlayerDebugMode) ──────────────────────

echo  [1/4] Enabling CEP extension support...

:: Write for CSXS 9, 10, 11 to cover Ae/Pr 2019 through latest
reg add "HKCU\Software\Adobe\CSXS.9"  /v PlayerDebugMode /t REG_SZ /d 1 /f >nul 2>&1
reg add "HKCU\Software\Adobe\CSXS.10" /v PlayerDebugMode /t REG_SZ /d 1 /f >nul 2>&1
reg add "HKCU\Software\Adobe\CSXS.11" /v PlayerDebugMode /t REG_SZ /d 1 /f >nul 2>&1

if !errorlevel! neq 0 (
    echo  [WARN] Registry write may have failed. Try running as Administrator if the panel doesn't appear.
) else (
    echo         OK
)

:: ── 3. Copy panel to CEP extensions folder ────────────────────────────────────

echo  [2/4] Installing panel to Adobe CEP extensions...

if not exist "%CEP_DIR%" mkdir "%CEP_DIR%"

:: Remove old install if present
if exist "%INSTALL_DIR%" (
    echo         Removing previous install...
    rmdir /s /q "%INSTALL_DIR%"
)

robocopy "%PANEL_SRC%" "%INSTALL_DIR%" /e /nfl /ndl /njh /njs >nul 2>&1
if !errorlevel! geq 8 (
    echo  [ERROR] Failed to copy panel files to %INSTALL_DIR%
    echo          Try closing After Effects / Premiere Pro first.
    echo.
    pause & exit /b 1
)
echo         OK  ^>  %INSTALL_DIR%

:: ── 4. Locate and copy blur-master binaries ───────────────────────────────────

echo  [3/4] Looking for blur-master installation...

set "BIN_DEST=%INSTALL_DIR%\bin"
set "BLUR_FOUND=0"

:: Common install locations
for %%L in (
    "%ProgramFiles%\blur"
    "%ProgramFiles(x86)%\blur"
    "%LOCALAPPDATA%\Programs\blur"
    "%APPDATA%\blur"
) do (
    if exist "%%~L\blur-cli.exe" (
        set "BLUR_ROOT=%%~L"
        set "BLUR_FOUND=1"
    )
)

if "!BLUR_FOUND!"=="1" (
    echo         Found blur-master at !BLUR_ROOT!
    echo  [4/4] Copying blur-master binaries...

    :: Copy blur-cli.exe
    copy /y "!BLUR_ROOT!\blur-cli.exe" "%BIN_DEST%\blur-cli.exe" >nul
    if exist "!BLUR_ROOT!\blur-gui.exe" copy /y "!BLUR_ROOT!\blur-gui.exe" "%BIN_DEST%\blur-gui.exe" >nul

    :: Copy lib\ contents (blur.py, ffmpeg/, vapoursynth/, etc.)
    if exist "!BLUR_ROOT!\lib" (
        robocopy "!BLUR_ROOT!\lib" "%BIN_DEST%" /e /nfl /ndl /njh /njs >nul 2>&1
    ) else (
        :: Older installs may have these at root level
        if exist "!BLUR_ROOT!\blur.py"     copy /y "!BLUR_ROOT!\blur.py" "%BIN_DEST%\blur.py" >nul
        if exist "!BLUR_ROOT!\ffmpeg"      robocopy "!BLUR_ROOT!\ffmpeg"     "%BIN_DEST%\ffmpeg"     /e /nfl /ndl /njh /njs >nul 2>&1
        if exist "!BLUR_ROOT!\vapoursynth" robocopy "!BLUR_ROOT!\vapoursynth" "%BIN_DEST%\vapoursynth" /e /nfl /ndl /njh /njs >nul 2>&1
    )

    echo         OK - binaries copied to %BIN_DEST%
    echo.
    echo  ===========================================
    echo   Installation complete!
    echo  ===========================================
    echo.
    echo   1. Restart After Effects or Premiere Pro
    echo   2. Open: Window ^> Extensions ^> Blur Panel
    echo.
) else (
    echo  [4/4] blur-master not found - manual step needed.
    echo.
    echo  ===========================================
    echo   Panel installed - binaries needed!
    echo  ===========================================
    echo.
    echo   The Blur Panel was installed, but blur-master binaries
    echo   were not found. Do one of the following:
    echo.
    echo   OPTION A - Install blur-master first (recommended):
    echo     1. Download blur-master from:
    echo        https://github.com/f0e/blur/releases/latest
    echo     2. Install it
    echo     3. Run THIS installer again
    echo.
    echo   OPTION B - Copy manually:
    echo     Copy from your blur-master install folder into:
    echo     %BIN_DEST%\
    echo.
    echo     Required files:
    echo       blur-cli.exe
    echo       blur.py
    echo       ffmpeg\  (folder)
    echo       vapoursynth\  (folder)
    echo.
    echo     See %BIN_DEST%\README.txt for details.
    echo.
)

pause
endlocal
