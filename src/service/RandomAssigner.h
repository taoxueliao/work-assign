// 接收词表和数量，打乱后无放回抽样，返回 Assignment 列表。

#pragma once

#include <vector>

#include "domain/Assignment.h"
#include "domain/Word.h"

class RandomAssigner {
public:
    std::vector<Assignment> assign(const std::vector<Word> &words, int count) const;
};
