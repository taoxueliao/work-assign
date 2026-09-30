"""Configure, build, and launch the Debug app."""

import ctypes
import os
import subprocess
import sys
from ctypes import wintypes
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
QT_BIN = Path(r"C:\Qt\5.15.2\mingw81_64\bin")
MINGW_BIN = Path(r"C:\Qt\Tools\mingw810_64\bin")
EXE = ROOT / "build" / "debug" / "app.exe"

PROCESS_QUERY_LIMITED_INFORMATION = 0x1000
PROCESS_TERMINATE = 0x0001
TH32CS_SNAPPROCESS = 0x00000002


class PROCESSENTRY32W(ctypes.Structure):
    _fields_ = [
        ("dwSize", wintypes.DWORD),
        ("cntUsage", wintypes.DWORD),
        ("th32ProcessID", wintypes.DWORD),
        ("th32DefaultHeapID", ctypes.POINTER(ctypes.c_ulong)),
        ("th32ModuleID", wintypes.DWORD),
        ("cntThreads", wintypes.DWORD),
        ("th32ParentProcessID", wintypes.DWORD),
        ("pcPriClassBase", ctypes.c_long),
        ("dwFlags", wintypes.DWORD),
        ("szExeFile", wintypes.WCHAR * 260),
    ]


def toolchain_env():
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(MINGW_BIN), str(QT_BIN), env.get("PATH", "")])
    return env


def run(args, env):
    completed = subprocess.run(args, cwd=ROOT, env=env)
    if completed.returncode != 0:
        sys.exit(completed.returncode)


def stop_running_debug_app():
    kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel32.CreateToolhelp32Snapshot.restype = wintypes.HANDLE
    kernel32.OpenProcess.restype = wintypes.HANDLE
    kernel32.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
    kernel32.QueryFullProcessImageNameW.restype = wintypes.BOOL
    kernel32.QueryFullProcessImageNameW.argtypes = [
        wintypes.HANDLE,
        wintypes.DWORD,
        wintypes.LPWSTR,
        ctypes.POINTER(wintypes.DWORD),
    ]
    kernel32.TerminateProcess.argtypes = [wintypes.HANDLE, wintypes.UINT]
    kernel32.CloseHandle.argtypes = [wintypes.HANDLE]

    snapshot = kernel32.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)
    if snapshot == wintypes.HANDLE(-1).value:
        return

    target = str(EXE).casefold()
    entry = PROCESSENTRY32W()
    entry.dwSize = ctypes.sizeof(PROCESSENTRY32W)
    found = []
    try:
        has_entry = kernel32.Process32FirstW(snapshot, ctypes.byref(entry))
        while has_entry:
            if entry.szExeFile.casefold() == "app.exe":
                handle = kernel32.OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, False, entry.th32ProcessID)
                if handle:
                    buffer = ctypes.create_unicode_buffer(32768)
                    size = wintypes.DWORD(len(buffer))
                    if kernel32.QueryFullProcessImageNameW(handle, 0, buffer, ctypes.byref(size)):
                        if buffer.value.casefold() == target:
                            found.append(entry.th32ProcessID)
                    kernel32.CloseHandle(handle)
            has_entry = kernel32.Process32NextW(snapshot, ctypes.byref(entry))
    finally:
        kernel32.CloseHandle(snapshot)

    for pid in found:
        handle = kernel32.OpenProcess(PROCESS_TERMINATE, False, pid)
        if handle:
            kernel32.TerminateProcess(handle, 1)
            kernel32.CloseHandle(handle)


def main():
    env = toolchain_env()
    stop_running_debug_app()
    run(["cmake", "--preset", "debug"], env)
    run(["cmake", "--build", "--preset", "debug"], env)
    if not EXE.exists():
        sys.exit(f"Debug executable was not produced: {EXE}")
    print(f"Launching {EXE}")
    subprocess.Popen([str(EXE)], cwd=EXE.parent, env=env)


if __name__ == "__main__":
    main()
