#include "Levels.hpp"
#include <unordered_map>

namespace GlobalList::Cache::Levels {
    constexpr auto LEVELS_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GlobalListLevel>> levelData;
    static std::unordered_map<int, int> levelIDByPlacement;

    std::vector<GlobalListLevel*> getDemonlist(int page) {
        if (page < 0) return {};

        std::vector<GlobalListLevel*> levels;
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

    void setDemonlist(std::vector<GlobalListLevel>&& levels) {
        for (auto& level : levels) {
            levelData[level.ingameID] = {std::move(level), std::chrono::steady_clock::now()};
            levelIDByPlacement[level.placement] = level.ingameID;
        }
    }


    GlobalListLevel* getLevel(int levelID) {
        auto it = levelData.find(levelID);
        if (
            it == levelData.end() ||
            isExpired(it->second.cachedAt, LEVELS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setLevel(GlobalListLevel&& level) {
        levelData[level.ingameID] = {std::move(level), std::chrono::steady_clock::now()};
        levelIDByPlacement[level.placement] = level.ingameID;
    }

    void clear() {
        levelData.clear();
        levelIDByPlacement.clear();
    }
};