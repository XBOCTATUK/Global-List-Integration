#include "GameLevels.hpp"
#include <unordered_map>

namespace GDL::Cache::GameLevels {
    constexpr auto GAME_LEVELS_TTL = std::chrono::minutes{30};
    static std::unordered_map<int, CacheEntry<GameLevel>> gameLevelData;

    GameLevel* getGameLevel(int levelID) {
        auto it = gameLevelData.find(levelID);
        if (
            it == gameLevelData.end() ||
            isExpired(it->second.cachedAt, GAME_LEVELS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setGameLevel(GameLevel&& gameLevel) {
        gameLevelData[gameLevel.levelID] = {std::move(gameLevel), std::chrono::steady_clock::now()};
    }

    void clear() {
        gameLevelData.clear();
    }
};