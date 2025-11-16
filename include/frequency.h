#pragma once

#include <unordered_map>

enum Frequency {
    ANNUAL = 1,
    QUARTERLY = 4,
    MONTHLY = 12,
};
static std::unordered_map<std::string, Frequency> const strToFrequency = {
    { "annual", Frequency::ANNUAL },
    { "quarterly", Frequency::QUARTERLY },
    { "monthly", Frequency::MONTHLY },
};
