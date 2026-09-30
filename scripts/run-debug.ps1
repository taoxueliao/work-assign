# Configure, build, and launch the Debug app.
$ErrorActionPreference = 'Stop'
$Root = Resolve-Path (Join-Path $PSScriptRoot '..')
Set-Location $Root

$env:PATH = "C:\Qt\Tools\mingw810_64\bin;C:\Qt\5.15.2\mingw81_64\bin;$env:PATH"

$Exe = Join-Path $Root 'build\app.exe'
Get-CimInstance Win32_Process -Filter "Name = 'app.exe'" -ErrorAction SilentlyContinue |
    Where-Object { $_.ExecutablePath -and ($_.ExecutablePath -ieq $Exe) } |
    ForEach-Object { Stop-Process -Id $_.ProcessId -Force -ErrorAction SilentlyContinue }

cmake --preset debug
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build --preset debug
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if (-not (Test-Path $Exe)) {
    Write-Error "Debug executable was not produced: $Exe"
}

Write-Host "Launching $Exe"
Start-Process -FilePath $Exe -WorkingDirectory (Split-Path $Exe)
