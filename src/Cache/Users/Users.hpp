#pragma once

#include "../Cache.hpp"
#include "../../Models/GDLUser.hpp"
#include "../../Models/GDLUserRecords.hpp"

namespace GDL::Cache::Users {
    GDLUser* getUser(int userID);
    void setUser(GDLUser&& user);
    void clearUsers();

    GDLUserRecords* getUserRecords(int userID);
    void setUserRecords(GDLUserRecords&& records);
    void clearRecords();

    void clearAll();
};