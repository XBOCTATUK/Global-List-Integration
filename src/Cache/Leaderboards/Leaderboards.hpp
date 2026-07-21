#pragma once

#include "../../Models/GDLCountry.hpp"
#include "../../Models/GDLCountryAdvanced.hpp"
#include "../../Models/GDLCountryUser.hpp"
#include "../../Models/UserLeaderboardKey.hpp"

namespace GDL::Cache::Leaderboards {
    const std::vector<int>* getUserLeaderboard(UserLeaderboardKey key);
    void setUserLeaderboard(UserLeaderboardKey key, std::vector<int>&& userQuery);
    void clearUserLeaderboard();

    const std::vector<GDLCountry>* getCountryLeaderboard(CountriesLeaderboardType type);
    void setCountryLeaderboard(CountriesLeaderboardType type, std::vector<GDLCountry>&& countries);
    void clearCountryLeaderboard();

    const std::vector<GDLCountryUser>* getMainCountryLeaderboard(const std::string& country);
    void setMainCountryLeaderboard(const std::string& country, std::vector<GDLCountryUser>&& users);
    void clearMainCountryLeaderboard();

    const GDLCountryAdvanced* getAdvancedCountryLeaderboard(const std::string& country);
    void setAdvancedCountryLeaderboard(const std::string& country, GDLCountryAdvanced&& data);
    void clearAdvancedCountryLeaderboard();

    void clearAll();
};