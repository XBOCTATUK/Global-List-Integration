#pragma once

#include "../Models/GDLCountry.hpp"
#include "../Models/APIError.hpp"

class CountryLeaderboardLoadedEvent : public geode::Event<CountryLeaderboardLoadedEvent, bool(geode::Result<std::vector<GDLCountry>, APIError>), CountriesLeaderboardType> {
public:
    using Event::Event;
};