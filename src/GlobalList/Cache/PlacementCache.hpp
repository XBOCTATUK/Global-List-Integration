#pragma once

#include <ctime>
#include <unordered_map>

#include "../../events/PositionEvent.hpp"

struct PlacementCache {
    int placement;
    time_t loadTime;
};

namespace GlobalList::Cache {
    void savePlacement(int levelID, int placement);
    void saveLevelWOPlacement(int levelID);

    int getPlacement(int levelID, bool checkStatus = true);
    bool isLevelWoPlacement(int levelID);

    size_t placementCacheSize();
    void placementCacheClear();
};