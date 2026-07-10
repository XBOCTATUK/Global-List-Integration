#pragma once

#include <vector>
#include "../Cache.hpp"
#include "../../Models/GDLLevel.hpp"
#include "../../Models/GDLCountry.hpp"

namespace GDL::Cache::Levels {
    std::vector<GDLLevel*>& getDemonlist();
    void setDemonlist(std::vector<GDLLevel>&& levels);

    GDLLevel* getLevel(int levelID);
    bool isLevelWOPlacement(int levelID);

    void setLevel(GDLLevel&& level);
    void setLevelWOPlacement(int levelID);

    void clear();
};