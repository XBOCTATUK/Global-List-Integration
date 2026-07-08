#pragma once

#include "../API.hpp"
#include "../../Utils/WedReq.hpp"

namespace GDL::API {
    void getVerificationToken(std::string username, std::string password);
    void getAccessToken(std::string verificationToken, std::string secretCode);
}