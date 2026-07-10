#pragma once

#include "../Cache.hpp"
#include "../../Models/GameLevel.hpp"

namespace GDL::Cache::GameLevels {
    GameLevel* getGameLevel(int levelID);
    void setGameLevel(GameLevel&& gameLevel);
    void clear();
};