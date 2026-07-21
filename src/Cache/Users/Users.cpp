#include "Users.hpp"
#include "../Cache.hpp"
#include <unordered_map>

namespace GDL::Cache::Users {
    constexpr auto USERS_TTL = std::chrono::minutes{30};
    constexpr auto USER_RECORDS_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GDLUser>> userData;
    static std::unordered_map<int, CacheEntry<GDLUserRecords>> userRecords;

    const GDLUser* getUser(int userID) {
        auto it = userData.find(userID);
        if (
            it == userData.end() ||
            isExpired(it->second.cachedAt, USERS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setUser(GDLUser&& user) {
        userData.insert_or_assign(
            user.id,
            CacheEntry{std::move(user), std::chrono::steady_clock::now()}
        );
    }

    void clearUsers() {
        userData.clear();
    }


    const GDLUserRecords* getUserRecords(int userID) {
        auto it = userRecords.find(userID);
        if (
            it == userRecords.end() ||
            isExpired(it->second.cachedAt, USER_RECORDS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setUserRecords(GDLUserRecords&& records) {
        userRecords.insert_or_assign(
            records.userID,
            CacheEntry{std::move(records), std::chrono::steady_clock::now()}
        );
    }

    void clearRecords() {
        userRecords.clear();
    }


    void clearAll() {
        clearUsers();
        clearRecords();
    }
};