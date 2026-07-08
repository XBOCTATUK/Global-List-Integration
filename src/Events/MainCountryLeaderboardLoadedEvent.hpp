#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListCountryUser.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class MainCountryLeaderboardLoadedEvent : public Event<MainCountryLeaderboardLoadedEvent, bool(Result<std::vector<GlobalListCountryUser>*, APIError>), std::string> {
public:
    using Event::Event;
};