#pragma once

#include "../Models/GDLCountryUser.hpp"
#include "../Models/APIError.hpp"

class MainCountryLeaderboardLoadedEvent : public geode::Event<MainCountryLeaderboardLoadedEvent, bool(geode::Result<const std::vector<GDLCountryUser>*, APIError>), std::string> {
public:
    using Event::Event;
};