#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"
#include "../../Cache/Users/Users.hpp"
#include "../../Events/UserLoadedEvent.hpp"
#include "../../Events/UserRecordsLoadedEvent.hpp"

namespace GDL::API::Users {
    void getUser(int userID, bool isFullInfoRequire = true);
    void getUserRecords(int userID);
};