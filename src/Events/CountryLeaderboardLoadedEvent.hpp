#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListCountry.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class CountryLeaderboardLoadedEvent : public Event<CountryLeaderboardLoadedEvent, bool(Result<std::vector<GlobalListCountry>*, APIError>), CountriesLeaderboardType> {
public:
    using Event::Event;
};