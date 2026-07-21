#pragma once

namespace GDL::API::Users {
    void getUser(int userID, bool isFullInfoRequire = true);
    void getUserRecords(int userID);
};