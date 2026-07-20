#pragma once

#include <vector>
#include "GDLBasicLevel.hpp"

struct GDLCountryAdvanced {
    int userCount;
    GDLBasicLevel hardestLevel;
    std::vector<GDLBasicLevel> mainList;
    std::vector<GDLBasicLevel> extendedList;
    std::vector<GDLBasicLevel> advancedList;
    std::vector<GDLBasicLevel> unboundedList;
    std::vector<GDLBasicLevel> progressList;
    std::vector<GDLBasicLevel> verifiedList;
};