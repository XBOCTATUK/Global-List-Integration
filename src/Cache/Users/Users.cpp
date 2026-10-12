#include "Users.hpp"
#include "../Cache.hpp"
#include <unordered_map>

namespace GDL::Cache::Users {
    constexpr auto USERS_TTL = std::chrono::minutes{30};
    constexpr auto USER_RECORDS_TTL = std::chrono::minutes{30};

    static std::unordered_map<int, CacheEntry<GDLUser>> userData;
    static std::unordered_map<int, CacheEntry<GDLUserRecords>> userRecords;
    static std::unordered_map<std::string, int> userIDByUsername;
    static std::unordered_map<int, std::chrono::steady_clock::time_point> fullUserCachedAt;

    const GDLUser* getUser(int userID) {
        auto it = userData.find(userID);
        if (
            it == userData.end() ||
            isExpired(it->second.cachedAt, USERS_TTL)
        ) return nullptr;

        return &it->second.value;
    }

    int getUserIDByUsername(std::string username) {
        auto it = userIDByUsername.find(username);
        if (it == userIDByUsername.end()) return -1;

        return it->second;
    }

    void setUser(GDLUser&& user) {
        auto it = userData.find(user.id);
        auto fullIt = fullUserCachedAt.find(user.id);
        if (
            !user.isFull() && it != userData.end() && it->second.value.isFull() &&
            fullIt != fullUserCachedAt.end() && !isExpired(fullIt->second, USERS_TTL)
        ) {
            auto& oldUser = it->second.value;
            user.isBanned = std::move(oldUser.isBanned);
            user.hardest = std::move(oldUser.hardest);
            user.mainList = std::move(oldUser.mainList);
            user.extendedList = std::move(oldUser.extendedList);
            user.advancedList = std::move(oldUser.advancedList);
            user.unboundedList = std::move(oldUser.unboundedList);
            user.progressList = std::move(oldUser.progressList);
            user.verifiedList = std::move(oldUser.verifiedList);
        }
        else if (user.isFull()) {
            fullUserCachedAt.insert_or_assign(user.id, std::chrono::steady_clock::now());
        }

        userIDByUsername.insert_or_assign(
            user.username,
            user.id
        );
        userData.insert_or_assign(
            user.id,
            CacheEntry{std::move(user), std::chrono::steady_clock::now()}
        );
    }

    void clearUsers() {
        userData.clear();
        userIDByUsername.clear();
        fullUserCachedAt.clear();
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
}