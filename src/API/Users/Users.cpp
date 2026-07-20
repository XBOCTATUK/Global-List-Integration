#include "Users.hpp"

namespace GDL::API::Users {
    void getUser(int userID, bool isFullInfoRequire) {
        auto cachedUser = GDL::Cache::Users::getUser(userID);
        if (cachedUser && (isFullInfoRequire ? cachedUser->isFull() : true)) {
            UserLoadedEvent(userID).send(
                Ok(cachedUser)
            );
            return;
        }

        Utils::WebReq(
            USER_EP,
            matjson::makeObject({ {"id", userID} }),
            matjson::Value::object(),
            [userID](matjson::Value data, APIError error) {
                if (error) {
                    UserLoadedEvent(userID).send(
                        Err(error)
                    );
                    return;
                }
                else if (!data.isObject() || data.size() == 0) {
                    log::error("The user data is incomplete or invalid.");
                    UserLoadedEvent(userID).send(
                        Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None})
                    );
                    return;
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
                
                auto hardest = GDLBasicLevel{hardestID, hardestName, hardestPlacement, hardestVideoURL};

                auto gdlUser = GDLUser{
                    userID, username, placement, points,
                    country, badge, isBanned, hardest
                };

                auto parseList = [](matjson::Value& levelData, optGDLBasicLevels& list, bool withPercent = false) {
                    for (const auto& level : levelData) {
                        int id = level["id"].asInt().unwrapOrDefault();
                        std::string name = level["name"].asString().unwrapOrDefault();
                        int placement = level["placement"].asInt().unwrapOrDefault();
                        std::string videoURL = level["video_url"].asString().unwrapOrDefault();
                        std::optional<int> percent =
                            withPercent
                            ? std::make_optional(level["percent"].asInt().unwrapOrDefault())
                            : std::nullopt;

                        if (!list.has_value()) {
                            list.emplace();
                        }
                        list.value().push_back({id, name, placement, videoURL, percent});
                    }
                };

                parseList(data["levels"]["main"], gdlUser.mainList);
                parseList(data["levels"]["extended"], gdlUser.extendedList);
                parseList(data["levels"]["advanced"], gdlUser.advancedList);
                parseList(data["levels"]["unbounded"], gdlUser.unboundedList);
                parseList(data["levels"]["progress"], gdlUser.progressList, true);
                parseList(data["levels"]["verified"], gdlUser.verifiedList);
                
                GDL::Cache::Users::setUser(std::move(gdlUser));
                UserLoadedEvent(userID).send(
                    Ok(GDL::Cache::Users::getUser(userID))
                );
            }
        );
    }

    void getUserRecords(int userID) {
        auto cachedUserRecords = GDL::Cache::Users::getUserRecords(userID);
        if (cachedUserRecords) {
            UserRecordsLoadedEvent(userID).send(
                Ok(cachedUserRecords)
            );
            return;
        }

        Utils::WebReq(
            USER_RECORDS_EP,
            matjson::makeObject({ {"user_id", userID} }),
            matjson::Value::object(),
            [userID](matjson::Value data, APIError error) {
                if (error) {
                    UserRecordsLoadedEvent(userID).send(
                        Err(error)
                    );
                    return;
                }
                else if (!data.isObject() || data.size() == 0) {
                    log::error("The user records data is incomplete or invalid.");
                    UserRecordsLoadedEvent(userID).send(
                        Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None})
                    );
                    return;
                }

                int totalCount = data["total_count"].asInt().unwrapOrDefault();
                int completedCount = data["completed_count"].asInt().unwrapOrDefault();
                std::vector<GDLRecord> records;

                for (const auto& record : data["records"]) {
                    int id = record["id"].asInt().unwrapOrDefault();
                    int percent = record["percent"].asInt().unwrapOrDefault();
                    std::string status = record["status"].asString().unwrapOrDefault();
                    std::string videoURL = record["video_url"].asString().unwrapOrDefault();
                    int internalID = record["level"]["id"].asInt().unwrapOrDefault();
                    std::string levelName = record["level"]["name"].asString().unwrapOrDefault();
                    int placement = record["level"]["placement"].asInt().unwrapOrDefault();

                    auto gdlRecord = GDLRecord{
                        id, percent, status, videoURL,
                        internalID, levelName, placement
                    };
                    records.push_back(gdlRecord);
                }

                auto userRecords = GDLUserRecords{userID, totalCount, completedCount, records};
                
                GDL::Cache::Users::setUserRecords(std::move(userRecords));
                UserRecordsLoadedEvent(userID).send(
                    Ok(GDL::Cache::Users::getUserRecords(userID))
                );
            }
        );
    }
};