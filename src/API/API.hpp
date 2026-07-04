#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

struct GlobalListBasicLevel {
    int id;
    std::string name;
    int placement;
    std::string videoURL;
    std::optional<int> percent;
};

struct GlobalListLevel {
    int id;
    int ingameID;
    int placement;
    std::string name;
    double points;
    int listPercent;
    int length;
    std::string holder;
    std::string verifier;
    int verifierID;
    std::string verificationURL;
    std::string dateCreated;
    
    std::optional<int> objects;
    std::optional<std::string> description;
    std::optional<std::string> creator;
    std::optional<std::string> songURL;
    std::optional<int> gameVersion;
    std::optional<bool> isCopyable;
    std::optional<std::string> password;
};

using optGlobalListBasicLevels = std::optional<std::vector<GlobalListBasicLevel>>;
struct GlobalListUser {
    int id;
    std::string username;
    int placement;
    double points;
    std::string country;
    std::string badge;

    std::optional<bool> isBanned;
    std::optional<GlobalListBasicLevel> hardest;
    optGlobalListBasicLevels mainList;
    optGlobalListBasicLevels extendedList;
    optGlobalListBasicLevels advancedList;
    optGlobalListBasicLevels unboundedList;
    optGlobalListBasicLevels progressList;
    optGlobalListBasicLevels verifiedList;
    optGlobalListBasicLevels uncompletedList;
};

struct GlobalListCountryUser {
    int id;
    std::string username;
    double points;
};

struct GlobalListCountry {
    std::string title;
    int placement;
    double points;
};

struct GlobalListCountryAdvanced {
    int userCount;
    GlobalListBasicLevel hardestLevel;
    std::vector<GlobalListBasicLevel> mainList;
    std::vector<GlobalListBasicLevel> extendedList;
    std::vector<GlobalListBasicLevel> advancedList;
    std::vector<GlobalListBasicLevel> unboundedList;
    std::vector<GlobalListBasicLevel> progressList;
    std::vector<GlobalListBasicLevel> verifiedList;
    std::vector<GlobalListBasicLevel> uncompletedList;
};

namespace GlobalList::API {
    static inline std::string baseURL = "https://api.demonlist.org";

    static inline std::string loginEP = baseURL + "/auth/login";
    static inline std::string verifyLoginEP = baseURL + "/auth/login/verify";

    static inline std::string levelEP = baseURL + "/level/classic/get";
    static inline std::string levelListEP = baseURL + "/level/classic/list";

    static inline std::string userEP = baseURL + "/user/get";
    static inline std::string userRecordsEP = baseURL + "/user/record/list";

    static inline std::string userLeaderboardEP = baseURL + "/leaderboard/user/list";
    static inline std::string countryLeaderboardEP = baseURL + "/leaderboard/country/list";
    static inline std::string getMainCountryLeaderboardEP = baseURL + "/leaderboard/country/main/get";
    static inline std::string getAdvanvedCountryLeaderboardEP = baseURL + "/leaderboard/country/advanced/get";
};