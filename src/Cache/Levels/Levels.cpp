#include "Levels.hpp"
#include <unordered_map>

namespace GDL::Cache::Levels {
    constexpr auto LEVELS_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GDLLevel>> levelData;
    static std::vector<GDLLevel*> levelList;
    static std::unordered_map<int, int> levelIDByPlacement;
    static std::vector<int> levelsWOPlacement;

    std::vector<GDLLevel*>& getDemonlist() {
        return levelList;
    }

    void setDemonlist(std::vector<GDLLevel>&& levels) {
        clear();

        for (auto& level : levels) {
            levelData[level.ingameID] = {std::move(level), std::chrono::steady_clock::now()};
            levelList.push_back(&levelData[level.ingameID].value);
            levelIDByPlacement[level.placement] = level.ingameID;
        }
    }


    GDLLevel* getLevel(int levelID) {
        auto it = levelData.find(levelID);
        if (
            it == levelData.end() ||
            isExpired(it->second.cachedAt, LEVELS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    bool isLevelWOPlacement(int levelID) {
        auto it = std::find(levelsWOPlacement.begin(), levelsWOPlacement.end(), levelID);
        return it != levelsWOPlacement.end();
    }

    void setLevel(GDLLevel&& level) {
        levelData[level.ingameID] = {std::move(level), std::chrono::steady_clock::now()};
        levelIDByPlacement[level.placement] = level.ingameID;
    }

    void setLevelWOPlacement(int levelID) {
        levelsWOPlacement.push_back(levelID);
    }

    void clear() {
        levelData.clear();
        levelList.clear();
        levelIDByPlacement.clear();
        levelsWOPlacement.clear();
    }
};