#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"
#include "../../Cache/Leaderboards/Leaderboards.hpp"
#include "../../Cache/Users/Users.hpp"
#include "../../Events/UserLeaderboardLoadedEvent.hpp"
#include "../../Events/CountryLeaderboardLoadedEvent.hpp"
#include "../../Events/MainCountryLeaderboardLoadedEvent.hpp"
#include "../../Events/AdvancedCountryLeaderboardLoadedEvent.hpp"

namespace GDL::API::Leaderboards {
    void getUserLeaderboard(int page = 1, const std::string& search = "", const std::string& country = "");
    void getCountryLeaderboard(CountriesLeaderboardType type);
    void getMainCountryLeaderboard(const std::string& country);
    void getAdvancedCountryLeaderboard(const std::string& country);
};