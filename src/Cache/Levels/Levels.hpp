#pragma once

#include <vector>
#include "../Cache.hpp"
#include "../../Models/GlobalListLevel.hpp"
#include "../../Models/GlobalListCountry.hpp"

namespace GlobalList::Cache::Levels {
    std::vector<GlobalListLevel*> getDemonlist(int page);
    void setDemonlist(std::vector<GlobalListLevel>&& levels);

    GlobalListLevel* getLevel(int levelID);
    void setLevel(GlobalListLevel&& level);

    void clear();
};