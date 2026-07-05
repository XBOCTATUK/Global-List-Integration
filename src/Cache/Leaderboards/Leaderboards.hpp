#pragma once

#include "../Cache.hpp"
#include "../../Models/GlobalListCountry.hpp"
#include "../../Models/GlobalListCountryAdvanced.hpp"
#include "../../Models/GlobalListCountryUser.hpp"
#include "../../Models/GlobalListUser.hpp"

namespace GlobalList::Cache::Leaderboards {
    std::vector<GlobalListUser*> getUsers(int page);
    void setUsers(std::vector<GlobalListUser>&& users);
    void clearUsers();

    std::vector<GlobalListCountry>* getCountry(CountriesLeaderboardType type);
    void setCountries(CountriesLeaderboardType type, std::vector<GlobalListCountry>&& countries);
    void clearCountries();

    std::vector<GlobalListCountryUser>* getCountryUsers(const std::string& country);
    void setCountryUsers(const std::string& country, std::vector<GlobalListCountryUser>&& users);
    void clearCountryUsers();

    GlobalListCountryAdvanced* getCountryAdvanced(const std::string& country);
    void setCountryAdvanced(const std::string& country, GlobalListCountryAdvanced&& data);
    void clearCountryAdvanced();

    void clearAll();
};