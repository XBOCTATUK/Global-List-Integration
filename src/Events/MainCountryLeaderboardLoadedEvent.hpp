#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListCountry.hpp"

using namespace geode::prelude;

class MainCountryLeaderboardLoadedEvent : public Event<MainCountryLeaderboardLoadedEvent, bool(std::vector<GlobalListCountryUser>*), std::string> {
public:
    using Event::Event;
};