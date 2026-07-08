#include "Levels.hpp"
#include <unordered_map>

namespace GDL::Cache::Levels {
    constexpr auto LEVELS_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GDLLevel>> levelData;
    static std::unordered_map<int, int> levelIDByPlacement;
    static std::vector<int> levelsWOPlacement;

    std::vector<GDLLevel*> getDemonlist(int page) {
        if (page < 0) return {};

        std::vector<GDLLevel*> levels;
        for (int i = 10 * page + 1; i < 10 * (page+1) + 1; i++) {
            auto levelID = levelIDByPlacement.find(i);
            if (
                levelID == levelIDByPlacement.end() ||
                isExpired(levelData[levelID->second].cachedAt, LEVELS_TTL)
            ) return {};

            levels.push_back(&levelData[levelID->second].value);
        }

        return levels;
    }

    void setDemonlist(std::vector<GDLLevel>&& levels) {
        for (auto& level : levels) {
            levelData[level.ingameID] = {std::move(level), std::chrono::steady_clock::now()};
            levelIDByPlacement[level.placement] = level.ingameID;
        }
    }


    GDLLevel* getLevel(int levelID) {
        auto it = levelData.find(levelID);
        if (
            it == levelData.end() /*||
            isExpired(it->second.cachedAt, LEVELS_TTL)*/
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
        levelIDByPlacement.clear();
        levelsWOPlacement.clear();
    }
};