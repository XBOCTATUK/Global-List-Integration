#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"
#include "../../Cache/Levels/Levels.hpp"
#include "../../Events/DemonlistLoadedEvent.hpp"
#include "../../Events/LevelLoadedEvent.hpp"

namespace GDL::API {
    void getDemonlist();
    void getLevel(int levelID);
};