#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Cache/Cache.hpp"

namespace GDL::API {
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