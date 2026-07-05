#pragma once

#include <string>
#include <vector>
#include <optional>
#include "GlobalListBasicLevel.hpp"

using optGlobalListBasicLevels = std::optional<std::vector<GlobalListBasicLevel>>;
struct GlobalListUser {
    int id;
    std::string username;
    int placement;
    double points;
    std::string country;
    std::string badge;

    std::optional<bool> isBanned;
    std::optional<GlobalListBasicLevel> hardest;
    optGlobalListBasicLevels mainList;
    optGlobalListBasicLevels extendedList;
    optGlobalListBasicLevels advancedList;
    optGlobalListBasicLevels unboundedList;
    optGlobalListBasicLevels progressList;
    optGlobalListBasicLevels verifiedList;
    optGlobalListBasicLevels uncompletedList;
};