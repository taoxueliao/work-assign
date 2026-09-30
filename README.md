# work-assign

用 Qt 5.15 和 QML 做的考研单词分配工具。从词库里按数量随机抽出单词，右侧同时显示释义，并可以导出成两个文本文件。

## 功能

- 选择月份、月次和单词量（1–300），点击「发布」后无放回抽取。
- 右侧按「内容 / 翻译」两列显示。一个单词有多条释义时写在同一行，词性在释义前面，例如 `v. 表演；举动；起作用 n. 行为，法令；一幕`。
- 「导出」弹出文件夹选择框，默认目录是「文档」，并记住上次选择的文件夹。
- 导出两个 UTF-8 文件：`月份_月次.txt`（单词）和 `月份_月次_translation.txt`（翻译），一行一条，顺序一致。

词库文件是 `resources/words/postgraduate.json`。

## 环境

- Qt 5.15.2，MinGW 8.1 64-bit
- CMake 3.16 或更高，并且在 `PATH` 里
- 编译器：`C:/Qt/Tools/mingw810_64/bin/g++.exe`
- Python 3 可选，只在不用 `.bat` 时需要，且只用标准库

## 换开发机要改的路径

只拷 `dist` 到另一台电脑运行时，不用改下面任何文件。下面是换一台用来编译的电脑，而且 Qt、MinGW 或 CMake 的安装位置和现在不同时要改的地方。装在原来的 `C:\Qt\5.15.2` 和 `C:\Qt\Tools\mingw810_64` 就不用动预设和脚本。

`debug`、`release` 预设继承 `qt5.15.2-mingw64`，所以编译路径只改这一处。

| 文件 | 改成新机器上的路径 |
| --- | --- |
| `CMakePresets.json` 里的 `qt5.15.2-mingw64` | `CMAKE_PREFIX_PATH`（Qt 目录）、`gcc.exe`、`g++.exe`、`mingw32-make.exe`，以及 `environment` 里的 `PATH` |
| `scripts/run-debug.ps1`、`scripts/publish-release.ps1` | Qt 的 `bin`、MinGW 的 `bin` |
| `scripts/run-debug.py`、`scripts/publish-release.py` | 同上，变量名是 `QT_BIN`、`MINGW_BIN` |
| `.vscode/settings.json` | `cmake.cmakePath`、`g++.exe`、`gdb.exe`，以及调试用的 `PATH`。只用脚本、不用编辑器调试时可以不改 |
| `.vscode/launch.json` | `gdb.exe`，以及调试用的 `PATH` |

源码、`CMakeLists.txt` 和 QML 里没有写这台电脑的安装路径。

## 构建产物

编译和打包只产生下面三处。`build/` 和 `dist/` 已写入 `.gitignore`，不会进仓库。

| 位置 | 作用 |
| --- | --- |
| `build/debug` | Debug 编译结果。调试和日常运行用 `build\debug\app.exe`，依赖本机已安装的 Qt。 |
| `build/release` | Release 编译结果。只作为打包输入，程序是 `build\release\app.exe`。 |
| `dist` | 最终发布目录。由 `build/release` 整理而来，带上 Qt 库、QML 插件和 MinGW 运行库后可以整夹拷走。 |

## 一键启动和发布

开发机需要上面的 Qt、MinGW 和 CMake。脚本会自己把 Qt 和 MinGW 的 `bin` 加进本次进程的 `PATH`，不用先改系统环境变量。

PowerShell 和 Python 做的是同一件事，用其中一种即可。Python 只用标准库。

### Debug：编译并启动

```bat
run-debug.bat
python scripts/run-debug.py
```

使用 CMake preset `debug`，输出到 `build\debug`。编译完成后直接打开 `build\debug\app.exe`。脚本结束时程序继续运行。再次启动前，会先关掉正在运行的这份 Debug 程序，避免 exe 被占用导致链接失败。

### Release：打包到 dist

```bat
publish-release.bat
python scripts/publish-release.py
```

使用 CMake preset `release`，先编译到 `build\release`，再把 `app.exe` 和运行依赖整理进 `dist\`。`windeployqt` 会带上 Qt 库、QML 插件和 MinGW 运行库。若三个编译器 DLL 没有被拷进去，脚本会再从 `C:\Qt\Tools\mingw810_64\bin` 补上。

把整个 `dist` 文件夹拷走就能在裸机上运行，目标电脑不需要安装 Qt，也不需要配置环境变量。

这套 MinGW 版 Qt 的正式库在 PE 头里会被 `windeployqt` 认成 debug。脚本因此不传 `--release`，否则 `platforms\qwindows.dll` 会被跳过，程序无法启动。

## 手动编译

CMake 需要在 `PATH` 里。Qt 和编译器路径写在 `CMakePresets.json` 的 `qt5.15.2-mingw64` 预设中，`debug` 和 `release` 都继承它。

Debug：

```bat
cmake --preset debug
cmake --build --preset debug
```

Release：

```bat
cmake --preset release
cmake --build --preset release
```

发布目录仍用上面的打包脚本生成，不要直接分发 `build\release`。

## 目录

```text
qml/Main.qml                 窗口，左右按 2:8 排布
qml/pane/LeftPane.qml        月份、月次、单词量、发布、导出
qml/pane/RightPane.qml       当前输出
src/repository/              读取词库
src/service/                 随机抽取
src/viewmodel/               提供给 QML 的列表和导出
scripts/run-debug.ps1        Debug 编译并启动（PowerShell）
scripts/run-debug.py         Debug 编译并启动（Python）
scripts/publish-release.ps1  编译 Release 并打包到 dist\（PowerShell）
scripts/publish-release.py   编译 Release 并打包到 dist\（Python）
run-debug.bat                双击，调用 PowerShell 启动脚本
publish-release.bat          双击，调用 PowerShell 发布脚本
build/debug                  生成：Debug，用于调试
build/release                生成：Release，用于打包
dist                         生成：最终发布目录
```
