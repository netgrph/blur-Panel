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
    echo  [2/2] Updating existing installation...
) else (
    echo  [2/2] Installing panel...
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
