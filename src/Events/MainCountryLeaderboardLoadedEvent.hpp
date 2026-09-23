#pragma once

#include "../Models/GDLCountryUser.hpp"
#include "../Models/APIError.hpp"

class MainCountryLeaderboardLoadedEvent : public geode::Event<MainCountryLeaderboardLoadedEvent, bool(geode::Result<std::vector<GDLCountryUser>, APIError>), std::string> {
public:
    using Event::Event;
};