#include "PlacementCache.hpp"

namespace GlobalList::Cache {
    static std::unordered_map<int, PlacementCache> placementCache;
    static std::vector<int> levelsWOPlacement;

    void savePlacement(int levelID, int placement) {
        placementCache[levelID] = {
            placement,
            std::time(nullptr)
        };
    }

    void saveLevelWOPlacement(int levelID) {
        levelsWOPlacement.push_back(levelID);
    }

    int getPlacement(int levelID, bool checkStatus) {
        auto it = placementCache.find(levelID);
        if (it == placementCache.end()) return -1;

        if (!checkStatus) return it->second.placement; 

        auto now = std::time(nullptr);
        if (now - it->second.loadTime > 1800) return -1;

        return it->second.placement;
    }

    bool isLevelWoPlacement(int levelID) {
        return std::find(levelsWOPlacement.begin(), levelsWOPlacement.end(), levelID) == levelsWOPlacement.end() ? false : true;
    }

    size_t placementCacheSize() {
        return placementCache.size();
    }
    void placementCacheClear() {
        placementCache.clear();
        levelsWOPlacement.clear();
    }
};