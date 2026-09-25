#pragma once

#include "GDLBasicLevel.hpp"

using OptGDLBasicLevels = std::optional<std::vector<GDLBasicLevel>>;
struct GDLUser {
    int id;
    std::string username;
    int placement;
    double points;
    std::string country;
    std::string badge;

    std::optional<bool> isBanned;
    std::optional<GDLBasicLevel> hardest;
    OptGDLBasicLevels mainList;
    OptGDLBasicLevels extendedList;
    OptGDLBasicLevels advancedList;
    OptGDLBasicLevels unboundedList;
    OptGDLBasicLevels progressList;
    OptGDLBasicLevels verifiedList;

    mutable OptGDLBasicLevels completedList;

    bool isFull() const {
        return isBanned.has_value();
    }

    const std::vector<GDLBasicLevel>& getCompletedList() const {
        if (!completedList) {
            completedList.emplace();

            auto append = [this](const OptGDLBasicLevels& list) {
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