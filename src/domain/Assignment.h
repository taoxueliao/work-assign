// 一次抽出的结果：序号、单词、翻译。

#pragma once

#include <string>

struct Assignment {
    int index = 0;
    std::string word;
    std::string translation;
};
