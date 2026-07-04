#include "Levels.hpp"

namespace GlobalList::API {
    void getDemonlist(int page) {
        Utils::WebReq(
            levelListEP,
            matjson::makeObject({ {"limit", 100}, {"offset", true ? 25 : 10 * (page-1)} }),
            matjson::Value::object(),
            [](matjson::Value data) {
                if (!data.contains("levels") || !data["levels"].isArray() || data["levels"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The demonlist data is incomplete or invalid. Please try again later."
                    );
                }

                for (const auto& level : data["levels"]) {
                    int id = level["id"].asInt().unwrapOrDefault();
                    int ingameID = level["ingame_id"].asInt().unwrapOrDefault();
                    int placement = level["placement"].asInt().unwrapOrDefault();
                    std::string name = level["name"].asString().unwrapOrDefault();
                    double points = level["points"].asDouble().unwrapOrDefault();
                    int listPercent = level["list_percent"].asInt().unwrapOrDefault();
                    int length = level["length"].asInt().unwrapOrDefault();
                    std::string holder = level["holder"].asString().unwrapOrDefault();
                    std::string verifier = level["verifier"]["username"].asString().unwrapOrDefault();
                    int verifierID = level["verifier"]["user_id"].asInt().unwrapOrDefault();
                    std::string verificationURL = level["verification_url"].asString().unwrapOrDefault();
                    std::string dateCreated = level["date_created"].asString().unwrapOrDefault();

                    auto globalListLevel = GlobalListLevel{
                        id, ingameID, placement, name, points,
                        listPercent, length, holder, verifier,
                        verifierID, verificationURL, dateCreated
                    };
                }
            }
        );
    }

    void getLevel(int levelID) {
        Utils::WebReq(
            levelEP,
            matjson::makeObject({ {"ingame_id", levelID} }),
            matjson::Value::object(),
            [](matjson::Value data) {
                if (!data.isObject() || data.size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The level data is incomplete or invalid. Please try again later."
                    );
                }

                int id = data["data"]["id"].asInt().unwrapOrDefault();
                int ingameID = data["data"]["ingame_id"].asInt().unwrapOrDefault();
                int placement = data["data"]["placement"].asInt().unwrapOrDefault();
                std::string name = data["data"]["name"].asString().unwrapOrDefault();
                double points = data["data"]["points"].asDouble().unwrapOrDefault();
                int listPercent = data["data"]["list_percent"].asInt().unwrapOrDefault();
                int length = data["data"]["length"].asInt().unwrapOrDefault();
                int objects = data["data"]["objects"].asInt().unwrapOrDefault();
                std::string description = data["data"]["description"].asString().unwrapOrDefault();
                std::string creator = data["data"]["creator"].asString().unwrapOrDefault();
                std::string holder = data["data"]["holder"].asString().unwrapOrDefault();
                std::string songURL = data["data"]["song_url"].asString().unwrapOrDefault();
                int gameVersion = data["data"]["game_version"].asInt().unwrapOrDefault();
                std::string verifier = data["data"]["verification"]["username"].asString().unwrapOrDefault();
                int verifierID = data["data"]["verification"]["user_id"].asInt().unwrapOrDefault();
                std::string verificationURL = data["data"]["verification"]["video_url"].asString().unwrapOrDefault();
                bool isCopyable = data["data"]["copy_info"]["is_copyable"].asBool().unwrapOrDefault();
                std::string password = data["data"]["copy_info"]["password"].asString().unwrapOrDefault();
                std::string dateCreated = data["data"]["date_created"].asString().unwrapOrDefault();

                auto globalListLevelData = GlobalListLevel{
                    id, ingameID, placement, name, points, listPercent,
                    length, holder, verifier, verifierID, verificationURL,
                    dateCreated, objects, description, creator,
                    songURL, gameVersion, isCopyable, password
                };
            }
        );
    }
};