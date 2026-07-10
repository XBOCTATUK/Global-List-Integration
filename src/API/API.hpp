#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Cache/Cache.hpp"

namespace GDL::API {
    inline const std::string BASE_URL = "https://api.demonlist.org";

    inline const std::string LOGIN_EP = BASE_URL + "/auth/login";
    inline const std::string VERIFY_LOGIN_EP = BASE_URL + "/auth/login/verify";

    inline const std::string LEVEL_EP = BASE_URL + "/level/classic/get";
    inline const std::string LEVEL_LIST_EP = BASE_URL + "/level/classic/list";

    inline const std::string USER_EP = BASE_URL + "/user/get";
    inline const std::string USER_RECORDS_EP = BASE_URL + "/user/record/list";

    inline const std::string USER_LEADERBOARD_EP = BASE_URL + "/leaderboard/user/list";
    inline const std::string COUNTRY_LEADERBOARD_EP = BASE_URL + "/leaderboard/country/list";
    inline const std::string MAIN_COUNTRY_LEADERBOARD_EP = BASE_URL + "/leaderboard/country/main/get";
    inline const std::string ADVANCED_COUNTRY_LEADERBOARD_EP = BASE_URL + "/leaderboard/country/advanced/get";
};