#pragma once

#include "../Cache.hpp"
#include "../../Models/GDLCountry.hpp"
#include "../../Models/GDLCountryAdvanced.hpp"
#include "../../Models/GDLCountryUser.hpp"
#include "../../Models/GDLUser.hpp"

namespace GDL::Cache::Leaderboards {
    std::vector<GDLUser*> getUsers(int page);
    void setUsers(std::vector<GDLUser>&& users);
    void clearUsers();

    std::vector<GDLCountry>* getCountry(CountriesLeaderboardType type);
    void setCountries(CountriesLeaderboardType type, std::vector<GDLCountry>&& countries);
    void clearCountries();

    std::vector<GDLCountryUser>* getCountryUsers(const std::string& country);
    void setCountryUsers(const std::string& country, std::vector<GDLCountryUser>&& users);
    void clearCountryUsers();

    GDLCountryAdvanced* getCountryAdvanced(const std::string& country);
    void setCountryAdvanced(const std::string& country, GDLCountryAdvanced&& data);
    void clearCountryAdvanced();

    void clearAll();
};