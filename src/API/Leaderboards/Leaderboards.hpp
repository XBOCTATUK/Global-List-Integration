#pragma once

#include "../../Models/GDLCountry.hpp"

namespace GDL::API::Leaderboards {
    void getUserLeaderboard(int page = 1, const std::string& search = "", const std::string& country = "");
    void getCountryLeaderboard(CountriesLeaderboardType type);
    void getMainCountryLeaderboard(const std::string& country);
    void getAdvancedCountryLeaderboard(const std::string& country);
};