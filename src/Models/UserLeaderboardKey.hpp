#pragma once

#include <string>
#include <Geode/utils/hash.hpp>

struct UserLeaderboardKey {
    int page;
    std::string search;
    std::string country;

    bool operator==(const UserLeaderboardKey&) const = default;
};

template<>
struct std::hash<UserLeaderboardKey> {
    size_t operator()(const UserLeaderboardKey& key) const noexcept {
        size_t seed;
        geode::hashCombine(seed, key.page);
        geode::hashCombine(seed, key.search);
        geode::hashCombine(seed, key.country);
        return seed;
    }
};