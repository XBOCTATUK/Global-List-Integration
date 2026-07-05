#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"
#include "../../Cache/Levels/Levels.hpp"
#include "../../Events/DemonlistLoadedEvent.hpp"
#include "../../Events/LevelLoadedEvent.hpp"

namespace GlobalList::API {
    void getDemonlist(int page);
    void getLevel(int levelID);
};