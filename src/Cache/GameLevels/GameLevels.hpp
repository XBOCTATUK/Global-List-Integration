#pragma once

#include "../Cache.hpp"
#include "../../Models/GameLevel.hpp"

namespace GDL::Cache::GameLevels {
    const GameLevel* getGameLevel(int levelID);
    void setGameLevel(GameLevel&& gameLevel);

    bool isOutToDate();
    void updateCachedAt();

    void clear();
};