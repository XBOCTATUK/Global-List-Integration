#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../../Models/GlobalListCountry.hpp"

using namespace geode::prelude;

class CountryLeaderboardLoadedEvent : public Event<CountryLeaderboardLoadedEvent, bool(std::vector<GlobalListCountry>*), CountriesLeaderboardType> {
public:
    using Event::Event;
};