#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GDLCountryAdvanced.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class AdvancedCountryLeaderboardLoadedEvent : public Event<AdvancedCountryLeaderboardLoadedEvent, bool(Result<const GDLCountryAdvanced*, APIError>), std::string> {
public:
    using Event::Event;
};