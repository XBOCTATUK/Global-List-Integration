#include "Leaderboards.hpp"
#include <unordered_map>

namespace GDL::Cache::Leaderboards {
    constexpr auto USERS_TTL = std::chrono::minutes{30};
    constexpr auto COUNTRIES_TTL = std::chrono::minutes{30};
    constexpr auto COUNTRY_USERS_TTL = std::chrono::minutes{30};
    constexpr auto COUNTRY_ADVANCED_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GDLUser>> userData;
    static std::unordered_map<CountriesLeaderboardType, CacheEntry<std::vector<GDLCountry>>> countryData;
    static std::unordered_map<std::string, CacheEntry<std::vector<GDLCountryUser>>> countryUserData;
    static std::unordered_map<std::string, CacheEntry<GDLCountryAdvanced>> countryAdvancedData;

    std::vector<GDLUser*> getUsers(int page) {
        if (page < 0) return {};

        std::vector<GDLUser*> users;
        for (int i = 10 * page + 1; i < 10 * (page+1) + 1; i++) {
            auto it = userData.find(i);
            if (
                it == userData.end() ||
                isExpired(it->second.cachedAt, USERS_TTL)
            ) return {};

            users.push_back(&it->second.value);
        }

        return users;
    }

    void setUsers(std::vector<GDLUser>&& users) {
        for (auto& user : users) {
            userData.insert_or_assign(
                user.placement,
                CacheEntry<GDLUser>{std::move(user), std::chrono::steady_clock::now()}
            );
        }
    }

    void clearUsers() {
        userData.clear();
    }


    std::vector<GDLCountry>* getCountry(CountriesLeaderboardType type) {
        auto it = countryData.find(type);
        if (
            it == countryData.end() ||
            isExpired(it->second.cachedAt, COUNTRIES_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setCountries(CountriesLeaderboardType type, std::vector<GDLCountry>&& countries) {
        countryData[type] = {std::move(countries), std::chrono::steady_clock::now()};
    }

    void clearCountries() {
        countryData.clear();
    }


    std::vector<GDLCountryUser>* getCountryUsers(const std::string& country) {
        auto it = countryUserData.find(country);
        if (
            it == countryUserData.end() ||
            isExpired(it->second.cachedAt, COUNTRY_USERS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setCountryUsers(const std::string& country, std::vector<GDLCountryUser>&& users) {
        countryUserData[country] = {std::move(users), std::chrono::steady_clock::now()};
    }

    void clearCountryUsers() {
        countryUserData.clear();
    }


    GDLCountryAdvanced* getCountryAdvanced(const std::string& country) {
        auto it = countryAdvancedData.find(country);
        if (
            it == countryAdvancedData.end() ||
            isExpired(it->second.cachedAt, COUNTRY_ADVANCED_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setCountryAdvanced(const std::string& country, GDLCountryAdvanced&& data) {
        countryAdvancedData[country] = {std::move(data), std::chrono::steady_clock::now()};
    }

    void clearCountryAdvanced() {
        countryAdvancedData.clear();
    }

    void clearAll() {
        clearUsers();
        clearCountries();
        clearCountryUsers();
        clearCountryAdvanced();
    }
};