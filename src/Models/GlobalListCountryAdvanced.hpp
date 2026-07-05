#pragma once

#include <vector>
#include "GlobalListBasicLevel.hpp"

struct GlobalListCountryAdvanced {
    int userCount;
    GlobalListBasicLevel hardestLevel;
    std::vector<GlobalListBasicLevel> mainList;
    std::vector<GlobalListBasicLevel> extendedList;
    std::vector<GlobalListBasicLevel> advancedList;
    std::vector<GlobalListBasicLevel> unboundedList;
    std::vector<GlobalListBasicLevel> progressList;
    std::vector<GlobalListBasicLevel> verifiedList;
    std::vector<GlobalListBasicLevel> uncompletedList;
};