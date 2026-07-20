#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GDLCountryUser.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class MainCountryLeaderboardLoadedEvent : public Event<MainCountryLeaderboardLoadedEvent, bool(Result<const std::vector<GDLCountryUser>*, APIError>), std::string> {
public:
    using Event::Event;
};