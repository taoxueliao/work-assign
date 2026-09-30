# Build Release and assemble a folder that runs without a Qt installation.
$ErrorActionPreference = 'Stop'
$Root = Resolve-Path (Join-Path $PSScriptRoot '..')
Set-Location $Root

$QtBin = 'C:\Qt\5.15.2\mingw81_64\bin'
$MingwBin = 'C:\Qt\Tools\mingw810_64\bin'
$env:PATH = "$MingwBin;$QtBin;$env:PATH"

cmake --preset release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build --preset release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$BuiltExe = Join-Path $Root 'build\release\app.exe'
if (-not (Test-Path $BuiltExe)) {
    Write-Error "Release executable was not produced: $BuiltExe"
}

$Dist = Join-Path $Root 'dist'
if (Test-Path $Dist) {
    Remove-Item $Dist -Recurse -Force
}
New-Item -ItemType Directory -Path $Dist | Out-Null
Copy-Item $BuiltExe $Dist

# This MinGW Qt kit marks its release libraries as debug in the PE header.
# Passing --release makes windeployqt skip every plugin, including qwindows.dll.
& (Join-Path $QtBin 'windeployqt.exe') `
    --qmldir (Join-Path $Root 'qml') `
    --compiler-runtime `
    (Join-Path $Dist 'app.exe')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

foreach ($Dll in @('libgcc_s_seh-1.dll', 'libstdc++-6.dll', 'libwinpthread-1.dll')) {
    $Destination = Join-Path $Dist $Dll
    if (-not (Test-Path $Destination)) {
        Copy-Item (Join-Path $MingwBin $Dll) $Destination
    }
}

Write-Host "Release package: $Dist"
