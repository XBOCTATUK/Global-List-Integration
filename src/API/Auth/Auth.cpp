#include "Auth.hpp"

namespace GlobalList::API {
    static std::string g_verificationToken;

    void getVerificationToken(std::string username, std::string password) {
        Utils::WebReq(
            loginEP,
            matjson::Value::object(),
            matjson::makeObject({ {"login", username}, {"password", password} }),
            [](matjson::Value data) {
                if (!data.contains("verification_token") || !data["verification_token"].isString() || data["verification_token"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The verification data is incomplete or invalid. Please try again later."
                    );
                }

                auto verificationToken = data["verification_token"].asString().unwrapOrDefault();
                g_verificationToken = verificationToken;
            }
        );
    }

    void getAccessToken(std::string secretCode) {
        Utils::WebReq(
            verifyLoginEP,
            matjson::Value::object(),
            matjson::makeObject({ {"verification_token", g_verificationToken}, {"secret_code", secretCode} }),
            [](matjson::Value data) {
                if (!data.contains("access_token") || !data["access_token"].isString() || data["access_token"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The JWT data is incomplete or invalid. Please try again later."
                    );
                }

                auto accessToken = data["access_token"].asString().unwrapOrDefault();
                Mod::get()->setSavedValue("jwt", accessToken);
            }
        );
    }
}