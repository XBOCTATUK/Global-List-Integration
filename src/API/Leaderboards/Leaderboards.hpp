#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"

enum CountriesLeaderboardType { Main, Advanced };

namespace GlobalList::API {
    void getUserLeaderboard(int page);
    void getCountryLeaderboard(CountriesLeaderboardType type);
    void getMainCountryLeaderboard(std::string country);
    void getAdvancedCountryLeaderboard(std::string country);
};