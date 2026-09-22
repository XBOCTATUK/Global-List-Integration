#pragma once

#include "../Models/GDLCountryAdvanced.hpp"
#include "../Models/APIError.hpp"

class AdvancedCountryLeaderboardLoadedEvent : public geode::Event<AdvancedCountryLeaderboardLoadedEvent, bool(geode::Result<const GDLCountryAdvanced*, APIError>), std::string> {
public:
    using Event::Event;
};