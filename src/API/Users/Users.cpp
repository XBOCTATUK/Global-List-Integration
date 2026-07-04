#include "Users.hpp"

namespace GlobalList::API {
    void getUser(int userID) {
        Utils::WebReq(
            userEP,
            matjson::makeObject({ {"id", userID} }),
            matjson::Value::object(),
            [userID](matjson::Value data) {
                if (!data.isObject() || data.size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The user data is incomplete or invalid. Please try again later."
                    );
                }

                std::string username = data["username"].asString().unwrapOrDefault();
                int placement = data["placement"].asInt().unwrapOrDefault();
                double points = data["points"].asDouble().unwrapOrDefault();
                std::string country = data["country"].asString().unwrapOrDefault();
                std::string badge = data["badge"].asString().unwrapOrDefault();
                bool isBanned = data["is_banned"].asBool().unwrapOrDefault();

                int hardestID = data["levels"]["hardest"]["id"].asInt().unwrapOrDefault();
                std::string hardestName = data["levels"]["hardest"]["name"].asString().unwrapOrDefault();
                int hardestPlacement = data["levels"]["hardest"]["placement"].asInt().unwrapOrDefault();
                std::string hardestVideoURL = data["levels"]["hardest"]["video_url"].asString().unwrapOrDefault();
                auto hardest = GlobalListBasicLevel{hardestID, hardestName, hardestPlacement, hardestVideoURL};

                auto globalListUser = GlobalListUser{
                    userID, username, placement, points,
                    country, badge, isBanned, hardest
                };

                auto parseList = [](matjson::Value& levelData, optGlobalListBasicLevels& list, bool withPercent = false) {
                    for (const auto& level : levelData) {
                        int id = level["id"].asInt().unwrapOrDefault();
                        std::string name = level["name"].asString().unwrapOrDefault();
                        int placement = level["placement"].asInt().unwrapOrDefault();
                        std::string videoURL = level["video_url"].asString().unwrapOrDefault();
                        std::optional<int> percent =
                            withPercent
                            ? std::make_optional(level["percent"].asInt().unwrapOrDefault())
                            : std::nullopt;

                        if (list.has_value()) {
                            list.value().push_back({id, name, placement, videoURL, percent});
                        }
                    }
                };

                parseList(data["levels"]["main"], globalListUser.mainList);
                parseList(data["levels"]["extended"], globalListUser.extendedList);
                parseList(data["levels"]["advanced"], globalListUser.advancedList);
                parseList(data["levels"]["unbounded"], globalListUser.unboundedList);
                parseList(data["levels"]["progress"], globalListUser.progressList, true);
                parseList(data["levels"]["verified"], globalListUser.verifiedList);
                parseList(data["levels"]["uncompleted"], globalListUser.uncompletedList);
            }
        );
    }

    void getUserRecords(int userID) {
        Utils::WebReq(
            userRecordsEP,
            matjson::makeObject({ {"user_id", userID} }),
            matjson::Value::object(),
            [userID](matjson::Value data) {
                if (!data.isObject() || data.size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The user data is incomplete or invalid. Please try again later."
                    );
                }

                std::string username = data["username"].asString().unwrapOrDefault();
                int placement = data["placement"].asInt().unwrapOrDefault();
                double points = data["points"].asDouble().unwrapOrDefault();
                std::string country = data["country"].asString().unwrapOrDefault();
                std::string badge = data["badge"].asString().unwrapOrDefault();
                bool isBanned = data["is_banned"].asBool().unwrapOrDefault();

                int hardestID = data["levels"]["hardest"]["id"].asInt().unwrapOrDefault();
                std::string hardestName = data["levels"]["hardest"]["name"].asString().unwrapOrDefault();
                int hardestPlacement = data["levels"]["hardest"]["placement"].asInt().unwrapOrDefault();
                std::string hardestVideoURL = data["levels"]["hardest"]["video_url"].asString().unwrapOrDefault();
                auto hardest = GlobalListBasicLevel{hardestID, hardestName, hardestPlacement, hardestVideoURL};

                auto globalListUser = GlobalListUser{
                    userID, username, placement, points,
                    country, badge, isBanned, hardest
                };

                auto parseList = [](matjson::Value& levelData, optGlobalListBasicLevels& list, bool withPercent = false) {
                    for (const auto& level : levelData) {
                        int id = level["id"].asInt().unwrapOrDefault();
                        std::string name = level["name"].asString().unwrapOrDefault();
                        int placement = level["placement"].asInt().unwrapOrDefault();
                        std::string videoURL = level["video_url"].asString().unwrapOrDefault();
                        std::optional<int> percent =
                            withPercent
                            ? std::make_optional(level["percent"].asInt().unwrapOrDefault())
                            : std::nullopt;

                        if (list.has_value()) {
                            list.value().push_back({id, name, placement, videoURL, percent});
                        }
                    }
                };

                parseList(data["levels"]["main"], globalListUser.mainList);
                parseList(data["levels"]["extended"], globalListUser.extendedList);
                parseList(data["levels"]["advanced"], globalListUser.advancedList);
                parseList(data["levels"]["unbounded"], globalListUser.unboundedList);
                parseList(data["levels"]["progress"], globalListUser.progressList, true);
                parseList(data["levels"]["verified"], globalListUser.verifiedList);
                parseList(data["levels"]["uncompleted"], globalListUser.uncompletedList);
            }
        );
    }
};