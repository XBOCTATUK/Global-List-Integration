#pragma once

#include <string>
#include <unordered_map>
#include <ctime>

#include "../../events/UserCachedEvent.hpp"
#include "../../events/LeaderboardCachedEvent.hpp"

struct LeaderboardUserCache {
    std::string username;
    float points;
    std::string country;
    time_t loadTime;
};

namespace GlobalList::Cache {
    void saveLeaderboardUser(const std::string& username, const LeaderboardUserCache& user);

    LeaderboardUserCache* getLeaderboardUser(const std::string& username);

    size_t leaderboardUserCacheSize();
    void leaderboardUserCacheClear();
}