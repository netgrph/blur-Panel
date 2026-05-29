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

if not exist "%PANEL_SRC%\bin\blur-cli.exe" (
    echo  [ERROR] blur-cli.exe is missing from com.blur.panel\bin\
    echo          Make sure you are using the full distribution package.
    echo.
    pause & exit /b 1
)

:: ── 2. Enable unsigned CEP extensions (PlayerDebugMode) ──────────────────────

echo  [1/2] Enabling CEP extension support...

reg add "HKCU\Software\Adobe\CSXS.9"  /v PlayerDebugMode /t REG_SZ /d 1 /f >nul 2>&1
reg add "HKCU\Software\Adobe\CSXS.10" /v PlayerDebugMode /t REG_SZ /d 1 /f >nul 2>&1
reg add "HKCU\Software\Adobe\CSXS.11" /v PlayerDebugMode /t REG_SZ /d 1 /f >nul 2>&1

echo         OK

:: ── 3. Copy panel (including bundled blur-master binaries) to CEP folder ─────

if exist "%INSTALL_DIR%" (
    echo  [2/4] Updating existing installation...
) else (
    echo  [2/4] Installing panel...
)

if not exist "%CEP_DIR%" mkdir "%CEP_DIR%"

:: /mir  = mirror source to dest (adds new, overwrites changed, removes deleted)
:: /xd config = skip the config\ folder so saved user settings are preserved
robocopy "%PANEL_SRC%" "%INSTALL_DIR%" /mir /xd "%PANEL_SRC%\config" /nfl /ndl /njh /njs >nul 2>&1
if !errorlevel! geq 8 (
    echo  [ERROR] Failed to copy panel files.
    echo          Close After Effects and Premiere Pro, then try again.
    echo.
    pause & exit /b 1
)

echo         OK  ^>  %INSTALL_DIR%

:: ── 4. Unpack bundled ffmpeg archive ─────────────────────────────────────────
::      ffmpeg binaries are shipped as ffmpeg.7z in bin\lib\ffmpeg\ to keep the
::      GitHub repo under file-size limits.
::      Extracted in place using the bundled 7z.exe from bin\vapoursynth\.
::      Re-extracted on every run because robocopy /mir would otherwise wipe
::      the extracted files on the next update.

echo  [3/4] Unpacking ffmpeg binaries...

set "SEVENZ=%INSTALL_DIR%\bin\vapoursynth\7z.exe"

if not exist "%SEVENZ%" (
    echo  [ERROR] 7z.exe not found at bin\vapoursynth\7z.exe
    echo.
    pause & exit /b 1
)

set "FFMPEG_ARCHIVE=%INSTALL_DIR%\bin\lib\ffmpeg\ffmpeg.7z"

if exist "%FFMPEG_ARCHIVE%" (
    "%SEVENZ%" x -y -o"%INSTALL_DIR%\bin\lib\ffmpeg" "%FFMPEG_ARCHIVE%" >nul
    if !errorlevel! neq 0 (
        echo  [ERROR] Failed to extract bin\lib\ffmpeg\ffmpeg.7z
        echo.
        pause & exit /b 1
    )
)

echo         OK

:: ── 5. Remove any previously-installed AE startup script ─────────────────────
::      (older installer versions copied blur_panel_template.jsx into every AE
::       Scripts\Startup folder. The panel now patches the OM inline at render
::       time, so the startup script is obsolete — clean it up.)

echo  [4/4] Cleaning up legacy AE startup script...

set "AE_APPDATA=%APPDATA%\Adobe\After Effects"
set "REMOVE_COUNT=0"

if exist "%AE_APPDATA%" (
    for /d %%v in ("%AE_APPDATA%\*") do (
        if exist "%%v\Scripts\Startup\blur_panel_template.jsx" (
            del /F /Q "%%v\Scripts\Startup\blur_panel_template.jsx" >nul 2>&1
            if !errorlevel! equ 0 set /a REMOVE_COUNT+=1
        )
    )
)

if !REMOVE_COUNT! gtr 0 (
    echo         OK  ^>  removed from !REMOVE_COUNT! AE version^(s^)
) else (
    echo         OK  ^>  nothing to clean up
)

echo.
echo  ===========================================
echo   Installation complete!
echo  ===========================================
echo.
echo   1. Restart After Effects or Premiere Pro
echo   2. Open: Window ^> Extensions ^> Blur Panel
echo.

pause
endlocal
