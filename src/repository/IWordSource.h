// 词库读取接口。具体来源由实现类决定。

#pragma once

#include <vector>

#include "domain/Word.h"

class IWordSource {
public:
    virtual ~IWordSource() = default;
    virtual std::vector<Word> load() = 0;
};
