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
- 编译器：`C:/Qt/Tools/mingw810_64/bin/g++.exe`

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
qml/Main.qml          窗口，左右按 2:8 排布
qml/pane/LeftPane.qml 月份、月次、单词量、发布、导出
qml/pane/RightPane.qml 当前输出
src/repository/       读取词库
src/service/          随机抽取
src/viewmodel/        提供给 QML 的列表和导出
```
