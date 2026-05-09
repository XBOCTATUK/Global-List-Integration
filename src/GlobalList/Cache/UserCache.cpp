#include "UserCache.hpp"

namespace GlobalList::Cache {
    static std::unordered_map<std::string, UserCache> userCache;

    void saveUser(const std::string& username, const UserCache& userData) {
        userCache[username] = userData;
    }

    UserCache* getUser(const std::string& username) {
        auto it = userCache.find(username);
        if (it == userCache.end()) return nullptr;

        auto now = std::time(nullptr);
        if (now - it->second.loadTime > 1800) return nullptr;

        return &it->second;
    }

    size_t userCacheSize() {
        return userCache.size();
    }

    void userCacheClear() {
        userCache.clear();
    }
};