#include "GameLevels.hpp"
#include <unordered_map>

namespace GDL::Cache::GameLevels {
    constexpr auto GAME_LEVELS_TTL = std::chrono::minutes{30};
    static std::unordered_map<int, GameLevel> gameLevelData;
    static std::optional<std::chrono::steady_clock::time_point> cachedAt;

    const GameLevel* getGameLevel(int levelID) {
        if (!cachedAt.has_value()) return nullptr;

        auto it = gameLevelData.find(levelID);
        if (
            it == gameLevelData.end() ||
            isExpired(cachedAt.value(), GAME_LEVELS_TTL)
        ) return nullptr;

        return &it->second;
    }

    void setGameLevel(GameLevel&& gameLevel) {
        gameLevelData[gameLevel.levelID] = std::move(gameLevel);
    }
    

    bool isOutToDate() {
        return cachedAt.has_value() ? std::chrono::steady_clock::now() - cachedAt.value() > GAME_LEVELS_TTL : true;
    }

    void updateCachedAt() {
        cachedAt = std::chrono::steady_clock::now();
    }

    void clear() {
        gameLevelData.clear();
    }
};