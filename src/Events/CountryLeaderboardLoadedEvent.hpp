#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GDLCountry.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class CountryLeaderboardLoadedEvent : public Event<CountryLeaderboardLoadedEvent, bool(Result<const std::vector<GDLCountry>*, APIError>), CountriesLeaderboardType> {
public:
    using Event::Event;
};