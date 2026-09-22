#pragma once

#include "../../Models/GDLUser.hpp"
#include "../../Models/GDLUserRecords.hpp"

namespace GDL::Cache::Users {
    const GDLUser* getUser(int userID);
    int getUserIDByUsername(std::string username);
    void setUser(GDLUser&& user);
    void clearUsers();

    const GDLUserRecords* getUserRecords(int userID);
    void setUserRecords(GDLUserRecords&& records);
    void clearRecords();

    void clearAll();
}