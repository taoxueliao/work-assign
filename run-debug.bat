@echo off
cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\run-debug.ps1"
if errorlevel 1 (
    echo Debug launch failed.
    pause
)
