#include "Users.hpp"

namespace GlobalList::API {
    void getUser(int userID) {
        auto cachedUser = GlobalList::Cache::Users::getUser(userID);
        if (cachedUser) {
            UserLoadedEvent(userID).send(cachedUser);
            return;
        }

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

                auto GDLUser = GlobalListUser{
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

                parseList(data["levels"]["main"], GDLUser.mainList);
                parseList(data["levels"]["extended"], GDLUser.extendedList);
                parseList(data["levels"]["advanced"], GDLUser.advancedList);
                parseList(data["levels"]["unbounded"], GDLUser.unboundedList);
                parseList(data["levels"]["progress"], GDLUser.progressList, true);
                parseList(data["levels"]["verified"], GDLUser.verifiedList);
                parseList(data["levels"]["uncompleted"], GDLUser.uncompletedList);
                
                GlobalList::Cache::Users::setUser(std::move(GDLUser));
                UserLoadedEvent(userID).send(GlobalList::Cache::Users::getUser(userID));
            }
        );
    }

    void getUserRecords(int userID) {
        auto cachedUserRecords = GlobalList::Cache::Users::getUserRecords(userID);
        if (cachedUserRecords) {
            UserRecordsLoadedEvent(userID).send(cachedUserRecords);
            return;
        }

        Utils::WebReq(
            userRecordsEP,
            matjson::makeObject({ {"user_id", userID} }),
            matjson::Value::object(),
            [userID](matjson::Value data) {
                if (!data.isObject() || data.size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The user records data is incomplete or invalid. Please try again later."
                    );
                }

                int totalCount = data["total_count"].asInt().unwrapOrDefault();
                int completedCount = data["completed_count"].asInt().unwrapOrDefault();
                std::vector<GlobalListRecord> records;

                for (const auto& record : data["records"]) {
                    int id = record["id"].asInt().unwrapOrDefault();
                    int percent = record["percent"].asInt().unwrapOrDefault();
                    std::string status = record["status"].asString().unwrapOrDefault();
                    std::string videoURL = record["video_url"].asString().unwrapOrDefault();
                    int internalID = record["level"]["id"].asInt().unwrapOrDefault();
                    std::string levelName = record["level"]["name"].asString().unwrapOrDefault();
                    int placement = record["level"]["placement"].asInt().unwrapOrDefault();

                    auto GDLRecord = GlobalListRecord{
                        id, percent, status, videoURL,
                        internalID, levelName, placement
                    };
                    records.push_back(GDLRecord);
                }

                auto userRecords = GlobalListUserRecords{userID, totalCount, completedCount, records};
                
                GlobalList::Cache::Users::setUserRecords(std::move(userRecords));
                UserRecordsLoadedEvent(userID).send(GlobalList::Cache::Users::getUserRecords(userID));
            }
        );
    }
};