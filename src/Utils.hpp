#pragma once

#include <Geode/Geode.hpp>
#include "GlobalList/Cache/PlacementCache.hpp"

namespace Utils {
    void removePlacement(const int levelID, CCNode* gdlLabel, CCNode* gdlIcon, const std::unordered_map<CCNode*, float> origPositions, const bool onLevelCell);
    void failure(int code);
};