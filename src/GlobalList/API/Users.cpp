#include "Users.hpp"

namespace GlobalList::API {
    extern std::string API_URL;

    void getUserRecords(int userID, std::string username);

    void getUser(std::string username, bool loadRecords) {
        spawn(web::WebRequest().get(fmt::format("{}/leaderboard/user/list?search={}", API_URL, username)),
            [username, loadRecords](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to get player info. Failed code: {}", value.code());
                    Utils::failure(value.code());
                    return;
                }

                auto data = value.json();

                matjson::Value json = data.ok().value();
                if (!json.contains("data") || !json["data"].contains("users") || !json["data"]["users"].isArray() || json["data"]["users"].size() == 0) {
                    Utils::failure(204);
                    return;
                }

                for (const auto& user : json["data"]["users"]) {
                    if (user["username"].asString().unwrapOr("") == username) {
                        int id = user["id"].asInt().unwrapOr(0);
                        int placement = user["placement"].asInt().unwrapOr(0);
                        double points = user["points"].asDouble().unwrapOr(0.0f);

                        auto userData = UserCache{
                            id, username, placement, points, {}, std::time(nullptr)
                        };
                        GlobalList::Cache::saveUser(username, userData);

                        if (loadRecords) Loader::get()->queueInMainThread([id, username]() { getUserRecords(id, username); });
                    }
                }
            }
        );
    }

    void getUserRecords(int userID, std::string username) {
        spawn(web::WebRequest().get(fmt::format("{}/user/record/list?user_id={}", API_URL, userID)),
            [username](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to get player records. Failed code: {}", value.code());
                    Utils::failure(value.code());
                    return;
                }

                auto data = value.json();

                matjson::Value json = data.ok().value();
                if (!json.contains("data") || !json["data"].contains("records") || !json["data"]["records"].isArray() || json["data"]["records"].size() == 0) {
                    Utils::failure(204);
                    return;
                }

                if (auto userData = GlobalList::Cache::getUser(username)) {
                    for (const auto& record : json["data"]["records"]) {
                        std::string name = record["level"]["name"].asString().unwrapOrDefault();
                        int levelID = record["level"]["ingame_id"].asInt().unwrapOrDefault();
                        int placement = record["level"]["placement"].asInt().unwrapOrDefault();

                        if (levelID != 0) userData->records.push_back( {name, levelID, placement} );

                        UserCachedEvent(username).send();
                    }
                }
            }
        );
    }
};