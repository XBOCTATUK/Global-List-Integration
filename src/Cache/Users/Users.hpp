#pragma once

#include "../Cache.hpp"
#include "../../Models/GlobalListUser.hpp"
#include "../../Models/GlobalListUserRecords.hpp"

namespace GlobalList::Cache::Users {
    GlobalListUser* getUser(int userID);
    void setUser(GlobalListUser&& user);
    void clearUsers();

    GlobalListUserRecords* getUserRecords(int userID);
    void setUserRecords(GlobalListUserRecords&& records);
    void clearRecords();

    void clearAll();
};