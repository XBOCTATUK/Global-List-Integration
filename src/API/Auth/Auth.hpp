#pragma once

namespace GDL::API::Auth {
    void getVerificationToken(const std::string& username, const std::string& password);
    void getAccessToken(const std::string& secretCode);
}