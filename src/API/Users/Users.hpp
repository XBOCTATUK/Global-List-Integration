#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"

namespace GlobalList::API {
    void getUser(int userID);
    void getUserRecords(int userID);
};