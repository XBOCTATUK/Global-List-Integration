#include "Leaderboards.hpp"
#include "../Cache.hpp"
#include <unordered_map>

namespace GDL::Cache::Leaderboards {
    constexpr auto USER_LEADERBOARD_TTL = std::chrono::minutes{30};
    constexpr auto COUNTRY_LEADERBOARD_TTL = std::chrono::minutes{30};
    constexpr auto COUNTRY_USERS_TTL = std::chrono::minutes{30};
    constexpr auto COUNTRY_ADVANCED_TTL = std::chrono::minutes{30};

    static std::unordered_map<UserLeaderboardKey, CacheEntry<std::vector<int>>> userQueryCache;
    static std::unordered_map<CountriesLeaderboardType, CacheEntry<std::vector<GDLCountry>>> countryData;
    static std::unordered_map<std::string, CacheEntry<std::vector<GDLCountryUser>>> countryUserData;
    static std::unordered_map<std::string, CacheEntry<GDLCountryAdvanced>> countryAdvancedData;

    const std::vector<int>* getUserLeaderboard(UserLeaderboardKey key) {
        auto it = userQueryCache.find(key);
        if (
            it == userQueryCache.end() ||
            isExpired(it->second.cachedAt, USER_LEADERBOARD_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setUserLeaderboard(UserLeaderboardKey key, std::vector<int>&& userQuery) {
        userQueryCache.insert_or_assign(
            key,
            CacheEntry{std::move(userQuery), std::chrono::steady_clock::now()}
        );
    }

    void clearUserLeaderboard() {
        userQueryCache.clear();
    }


    const std::vector<GDLCountry>* getCountryLeaderboard(CountriesLeaderboardType type) {
        auto it = countryData.find(type);
        if (
            it == countryData.end() ||
            isExpired(it->second.cachedAt, COUNTRY_LEADERBOARD_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setCountryLeaderboard(CountriesLeaderboardType type, std::vector<GDLCountry>&& countries) {
        countryData.insert_or_assign(
            type,
            CacheEntry{std::move(countries), std::chrono::steady_clock::now()}
        );
    }

    void clearCountryLeaderboard() {
        countryData.clear();
    }


    const std::vector<GDLCountryUser>* getMainCountryLeaderboard(const std::string& country) {
        auto it = countryUserData.find(country);
        if (
            it == countryUserData.end() ||
            isExpired(it->second.cachedAt, COUNTRY_USERS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setMainCountryLeaderboard(const std::string& country, std::vector<GDLCountryUser>&& users) {
        countryUserData.insert_or_assign(
            country,
            CacheEntry{std::move(users), std::chrono::steady_clock::now()}
        );
    }

    void clearMainCountryLeaderboard() {
        countryUserData.clear();
    }


    const GDLCountryAdvanced* getAdvancedCountryLeaderboard(const std::string& country) {
        auto it = countryAdvancedData.find(country);
        if (
            it == countryAdvancedData.end() ||
            isExpired(it->second.cachedAt, COUNTRY_ADVANCED_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setAdvancedCountryLeaderboard(const std::string& country, GDLCountryAdvanced&& data) {
        countryAdvancedData.insert_or_assign(
            country,
            CacheEntry{std::move(data), std::chrono::steady_clock::now()}
        );
    }

    void clearAdvancedCountryLeaderboard() {
        countryAdvancedData.clear();
    }
    

    void clearAll() {
        clearUserLeaderboard();
        clearCountryLeaderboard();
        clearMainCountryLeaderboard();
        clearAdvancedCountryLeaderboard();
    }
};