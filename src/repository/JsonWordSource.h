// 从 postgraduate.json 解析出 Word 列表。同一单词的多条释义合成一行，词性写在释义前面。

#pragma once

#include "repository/IWordSource.h"

class JsonWordSource : public IWordSource {
public:
    std::vector<Word> load() override;
};
