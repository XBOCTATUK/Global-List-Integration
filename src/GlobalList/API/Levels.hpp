#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../../events/PopulateListEvent.hpp"
#include "../Cache/PlacementCache.hpp"
#include "../Levels.hpp"
#include "../../Utils.hpp"

namespace GlobalList::API {
    void getDemonlist();
    void getLevelPlacement(int levelID);
};