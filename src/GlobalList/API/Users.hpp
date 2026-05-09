#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

#include "../Cache/UserCache.hpp"
#include "../../Utils.hpp"

namespace GlobalList::API {
    void getUser(std::string username, bool loadRecords);
};