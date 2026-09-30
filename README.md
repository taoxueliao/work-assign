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
- CMake 3.16 或更高
- Python 3（只用标准库，没有第三方包）
- 编译器：`C:/Qt/Tools/mingw810_64/bin/g++.exe`

## 一键启动和发布

开发机需要上面的 Qt、MinGW 和 CMake，并且能运行 `python`。脚本会自己把 Qt 和 MinGW 的 `bin` 加进本次进程的 `PATH`，不用先改系统环境变量。

PowerShell 和 Python 做的是同一件事，用其中一种即可。

### Debug：编译并启动

```bat
run-debug.bat
python scripts/run-debug.py
```

使用 CMake preset `debug`，编译 `build\app.exe` 后直接打开窗口。脚本结束时程序继续运行。再次启动前，会先关掉正在运行的这份 Debug 程序，避免 exe 被占用导致链接失败。

这个目录里的程序依赖本机 Qt。它不能单独拷到没装环境的电脑上。

### Release：打包

```bat
publish-release.bat
python scripts/publish-release.py
```

使用 CMake preset `release`，先编译 `build\release\app.exe`，再整理到 `dist\`。`windeployqt` 会带上 Qt 库、QML 插件和 MinGW 运行库。若三个编译器 DLL 没有被拷进去，脚本会再从 `C:\Qt\Tools\mingw810_64\bin` 补上。

把整个 `dist` 文件夹拷走就能在裸机上运行，目标电脑不需要安装 Qt，也不需要配置环境变量。

这套 MinGW 版 Qt 的正式库在 PE 头里会被 `windeployqt` 认成 debug。脚本因此不传 `--release`，否则 `platforms\qwindows.dll` 会被跳过，程序无法启动。

## 编译

```bat
cmake -S . -B build -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_PREFIX_PATH=C:/Qt/5.15.2/mingw81_64 ^
  -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw810_64/bin/g++.exe ^
  -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/mingw810_64/bin/mingw32-make.exe ^
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5

cmake --build build
```

运行前把 Qt 和 MinGW 的 `bin` 加进 `PATH`：

```bat
set PATH=C:\Qt\5.15.2\mingw81_64\bin;C:\Qt\Tools\mingw810_64\bin;%PATH%
build\app.exe
```

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
scripts/publish-release.ps1  打包 Release 到 dist\（PowerShell）
scripts/publish-release.py   打包 Release 到 dist\（Python）
run-debug.bat                双击，调用 PowerShell 启动脚本
publish-release.bat          双击，调用 PowerShell 发布脚本
```
