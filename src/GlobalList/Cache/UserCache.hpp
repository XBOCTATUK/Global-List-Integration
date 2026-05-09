#pragma once

#include <string>
#include <unordered_map>
#include <ctime>

#include "../../events/UserCachedEvent.hpp"

struct UserLevel {
    std::string name;
	int levelID;
	int placement;
};

struct UserCache {
    int id;
    std::string username;
    int placement;
    double points;
    std::vector<UserLevel> records;
    time_t loadTime;
};

namespace GlobalList::Cache {
    void saveUser(const std::string& username, const UserCache& userData);

    UserCache* getUser(const std::string& username);

    size_t userCacheSize();
    void userCacheClear();
};