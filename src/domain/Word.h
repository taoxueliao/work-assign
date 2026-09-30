// 一条词：单词与翻译。不读文件，也不依赖界面。

#pragma once

#include <string>

struct Word {
    std::string word;
    std::string translation;
};
