#include "LevelCache.hpp"

namespace GlobalList::Cache {
    static std::unordered_map<int, LevelData> allLevelData;
    static time_t loadTime;

    void saveLevelData(int levelID, const LevelData& levelData) {
        allLevelData[levelID] = levelData;
    }

    LevelData* getLevelData(int levelID) {
        auto it = allLevelData.find(levelID);
        if (it == allLevelData.end()) return nullptr;

        auto now = std::time(nullptr);
        if (loadTime > 1800) return nullptr;

        return &it->second;
    }

    size_t levelDataSize() {
        return allLevelData.size();
    }

    void levelDataClear() {
        allLevelData.clear();
    }

    void updateLevelLoadTime() {
        loadTime = std::time(nullptr);
    }
}