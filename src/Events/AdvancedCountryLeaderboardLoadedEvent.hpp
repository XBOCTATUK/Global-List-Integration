#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListCountry.hpp"

using namespace geode::prelude;

class AdvancedCountryLeaderboardLoadedEvent : public Event<AdvancedCountryLeaderboardLoadedEvent, bool(GlobalListCountryAdvanced*), std::string> {
public:
    using Event::Event;
};