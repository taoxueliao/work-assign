"""Build Release and assemble a folder that runs without a Qt installation."""

import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
QT_BIN = Path(r"C:\Qt\5.15.2\mingw81_64\bin")
MINGW_BIN = Path(r"C:\Qt\Tools\mingw810_64\bin")
BUILT_EXE = ROOT / "build" / "release" / "app.exe"
DIST = ROOT / "dist"
MINGW_DLLS = ("libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")


def toolchain_env():
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(MINGW_BIN), str(QT_BIN), env.get("PATH", "")])
    return env


def run(args, env):
    completed = subprocess.run(args, cwd=ROOT, env=env)
    if completed.returncode != 0:
        sys.exit(completed.returncode)


def main():
    env = toolchain_env()
    run(["cmake", "--preset", "release"], env)
    run(["cmake", "--build", "--preset", "release"], env)
    if not BUILT_EXE.exists():
        sys.exit(f"Release executable was not produced: {BUILT_EXE}")

    if DIST.exists():
        shutil.rmtree(DIST)
    DIST.mkdir()
    shutil.copy2(BUILT_EXE, DIST / "app.exe")

    # This MinGW Qt kit marks its release libraries as debug in the PE header.
    # Passing --release makes windeployqt skip every plugin, including qwindows.dll.
    run(
        [
            str(QT_BIN / "windeployqt.exe"),
            "--qmldir",
            str(ROOT / "qml"),
            "--compiler-runtime",
            str(DIST / "app.exe"),
        ],
        env,
    )

    for dll in MINGW_DLLS:
        destination = DIST / dll
        if not destination.exists():
            shutil.copy2(MINGW_BIN / dll, destination)

    print(f"Release package: {DIST}")


if __name__ == "__main__":
    main()
