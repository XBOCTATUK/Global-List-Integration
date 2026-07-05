#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"
#include "../../Cache/Leaderboards/Leaderboards.hpp"
#include "../../Events/UserLeaderboardLoadedEvent.hpp"
#include "../../Events/CountryLeaderboardLoadedEvent.hpp"
#include "../../Events/MainCountryLeaderboardLoadedEvent.hpp"
#include "../../Events/AdvancedCountryLeaderboardLoadedEvent.hpp"

namespace GlobalList::API {
    void getUserLeaderboard(int page);
    void getCountryLeaderboard(CountriesLeaderboardType type);
    void getMainCountryLeaderboard(std::string country);
    void getAdvancedCountryLeaderboard(std::string country);
};