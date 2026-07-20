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

    mutable optGDLBasicLevels completedList;

    bool isFull() const {
        return isBanned.has_value();
    }

    const std::vector<GDLBasicLevel>& getCompletedList() const {
        if (!completedList) {
            completedList.emplace();

            auto append = [this](const optGDLBasicLevels& list) {
                if (!list.has_value()) return;

                completedList->insert(completedList->end(), list->begin(), list->end());
            };

            append(mainList);
            append(extendedList);
            append(advancedList);
            append(unboundedList);
        }

        return *completedList;
    }
};