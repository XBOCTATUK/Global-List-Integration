#include "Users.hpp"
#include <unordered_map>

namespace GlobalList::Cache::Users {
    constexpr auto USERS_TTL = std::chrono::minutes{30};
    constexpr auto USER_RECORDS_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GlobalListUser>> userData;
    static std::unordered_map<int, CacheEntry<GlobalListUserRecords>> userRecords;

    GlobalListUser* getUser(int userID) {
        auto it = userData.find(userID);
        if (
            it == userData.end() ||
            isExpired(it->second.cachedAt, USERS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setUser(GlobalListUser&& user) {
        userData[user.id] = {std::move(user), std::chrono::steady_clock::now()};
    }

    void clearUsers() {
        userData.clear();
    }


    GlobalListUserRecords* getUserRecords(int userID) {
        auto it = userRecords.find(userID);
        if (
            it == userRecords.end() ||
            isExpired(it->second.cachedAt, USER_RECORDS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    void setUserRecords(GlobalListUserRecords&& records) {
        userRecords[records.userID] = {std::move(records), std::chrono::steady_clock::now()};
    }

    void clearRecords() {
        userRecords.clear();
    }


    void clearAll() {
        clearUsers();
        clearRecords();
    }
};