#pragma once

#include <Geode/utils/hash.hpp>

struct UserLeaderboardKey {
    int page = 0;
    std::string search;
    std::string country;
    int limit = 0;

    bool operator==(const UserLeaderboardKey&) const = default;
};

template<>
struct std::hash<UserLeaderboardKey> {
    size_t operator()(const UserLeaderboardKey& key) const noexcept {
        size_t seed = 0;
        geode::hashCombine(seed, key.page);
        geode::hashCombine(seed, key.search);
        geode::hashCombine(seed, key.country);
        geode::hashCombine(seed, key.limit);
        return seed;
    }
};