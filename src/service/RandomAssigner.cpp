#include "service/RandomAssigner.h"

#include <algorithm>
#include <random>

std::vector<Assignment> RandomAssigner::assign(const std::vector<Word> &words, int count) const
{
    if (words.empty() || count <= 0)
        return {};

    std::vector<Word> shuffled = words;
    std::mt19937 rng{std::random_device{}()};
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    const int n = std::min(count, static_cast<int>(shuffled.size()));
    std::vector<Assignment> result;
    result.reserve(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        Assignment item;
        item.index = i + 1;
        item.word = shuffled[static_cast<size_t>(i)].word;
        item.translation = shuffled[static_cast<size_t>(i)].translation;
        result.push_back(std::move(item));
    }
    return result;
}
