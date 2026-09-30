@echo off
cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\publish-release.ps1"
if errorlevel 1 (
    echo Release publish failed.
) else (
    echo Release package is in dist\
)
pause
