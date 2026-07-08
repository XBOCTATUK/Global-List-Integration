#pragma once

#include <string>
#include <vector>
#include <optional>
#include "GDLBasicLevel.hpp"

using optGDLBasicLevels = std::optional<std::vector<GDLBasicLevel>>;
struct GDLUser {
    int id;
    std::string username;
    int placement;
    double points;
    std::string country;
    std::string badge;

    std::optional<bool> isBanned;
    std::optional<GDLBasicLevel> hardest;
    optGDLBasicLevels mainList;
    optGDLBasicLevels extendedList;
    optGDLBasicLevels advancedList;
    optGDLBasicLevels unboundedList;
    optGDLBasicLevels progressList;
    optGDLBasicLevels verifiedList;
    optGDLBasicLevels uncompletedList;
};