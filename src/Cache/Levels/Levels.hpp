#pragma once

#include "../../Models/GDLLevel.hpp"
#include <vector>

namespace GDL::Cache::Levels {
    const std::vector<int>& getDemonlist();
    void setDemonlist(std::vector<GDLLevel>&& levels);

    const GDLLevel* getLevel(int levelID);
    void setLevel(GDLLevel&& level);
    
    bool isLevelWOPlacement(int levelID);
    void setLevelWOPlacement(int levelID);

    void clear();
};