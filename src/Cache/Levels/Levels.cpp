#include "Levels.hpp"
#include "../Cache.hpp"
#include <unordered_map>

namespace GDL::Cache::Levels {
    constexpr auto LEVELS_TTL = std::chrono::minutes{30};

    static CacheEntry<std::vector<int>> levelList;
    static std::unordered_map<int, CacheEntry<GDLLevel>> levelData;
    static std::vector<int> levelsWOPlacement;

    const std::vector<int>* getDemonlist() {
        if (
            isExpired(levelList.cachedAt, LEVELS_TTL)
        ) return nullptr;

        return &levelList.value;
    }

    void setDemonlist(std::vector<GDLLevel>&& levels) {
        clear();

        for (auto& level : levels) {
            levelData.insert_or_assign(
                level.ingameID,
                CacheEntry{std::move(level), std::chrono::steady_clock::now()}
            );
            levelList.value.push_back(levelData[level.ingameID].value.ingameID);
        }
        levelList.cachedAt = std::chrono::steady_clock::now();
    }


    const GDLLevel* getLevel(int levelID) {
        auto it = levelData.find(levelID);
        if (
            it == levelData.end() ||
            isExpired(it->second.cachedAt, LEVELS_TTL)
        ) return nullptr;

        return &it->second.value;
    }
    
    void setLevel(GDLLevel&& level) {
        levelData.insert_or_assign(
            level.ingameID,
            CacheEntry{std::move(level), std::chrono::steady_clock::now()}
        );
        levelList.value.push_back(level.ingameID);
    }

    
    bool isLevelWOPlacement(int levelID) {
        auto it = std::find(levelsWOPlacement.begin(), levelsWOPlacement.end(), levelID);
        return it != levelsWOPlacement.end();
    }

    void setLevelWOPlacement(int levelID) {
        levelsWOPlacement.push_back(levelID);
    }
    

    void clear() {
        levelData.clear();
        levelList.value.clear();
        levelList.cachedAt = std::chrono::steady_clock::time_point{};
        levelsWOPlacement.clear();
    }
}