#include "Leaderboards.hpp"

namespace GlobalList::API {
    void getUserLeaderboard(int page) {
        Utils::WebReq(
            userLeaderboardEP,
            matjson::makeObject({ {"offset", 50 * (page-1)} }),
            matjson::Value::object(),
            [](matjson::Value data) {
                if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The player leaderboard data is incomplete or invalid. Please try again later."
                    );
                }

                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    int placement = user["placement"].asInt().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();
                    std::string country = user["country"].asString().unwrapOrDefault();
                    std::string badge = user["badge"].asString().unwrapOrDefault();

                    auto globalListUser = GlobalListUser{id, username, placement, points, country, badge};
                }
            }
        );
    }

    void getCountryLeaderboard(CountriesLeaderboardType type) {
        auto typeStr = type == CountriesLeaderboardType::Main ? "main" : "advanced";

        Utils::WebReq(
            countryLeaderboardEP,
            matjson::makeObject({ {"type", typeStr} }),
            matjson::Value::object(),
            [](matjson::Value data) {
                if (!data.contains("countries") || !data["countries"].isArray() || data["countries"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The countries leaderboard data is incomplete or invalid. Please try again later."
                    );
                }

                for (const auto& country : data["countries"]) {
                    std::string title = country["title"].asString().unwrapOrDefault();
                    int placement = country["placement"].asInt().unwrapOrDefault();
                    double points = country["points"].asDouble().unwrapOrDefault();

                    auto globalListCountry = GlobalListCountry{title, placement, points};
                }
            }
        );
    }

    void getMainCountryLeaderboard(std::string country) {
        Utils::WebReq(
            getMainCountryLeaderboardEP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [](matjson::Value data) {
                if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The main country leaderboard data is incomplete or invalid. Please try again later."
                    );
                }

                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();

                    auto globalListCountryUser = GlobalListCountryUser{id, username, points};
                }
            }
        );
    }

    void getAdvancedCountryLeaderboard(std::string country) {
        Utils::WebReq(
            getAdvanvedCountryLeaderboardEP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [](matjson::Value data) {
                if (!data.contains("levels") || !data["levels"].isObject() || data["levels"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The advanced country leaderboard data is incomplete or invalid. Please try again later."
                    );
                }
                
                auto globalListCountryAdvanced = GlobalListCountryAdvanced{};

                int id = data["levels"]["hardest"]["id"].asInt().unwrapOrDefault();
                std::string name = data["levels"]["hardest"]["name"].asString().unwrapOrDefault();
                int placement = data["levels"]["hardest"]["placement"].asInt().unwrapOrDefault();
                std::string videoURL = data["levels"]["hardest"]["video_url"].asString().unwrapOrDefault();

                globalListCountryAdvanced.hardestLevel = {id, name, placement, videoURL};

                auto parseList = [](matjson::Value& levelData, std::vector<GlobalListBasicLevel>& list, bool withPercent = false) {
                    for (const auto& level : levelData) {
                        int id = level["id"].asInt().unwrapOrDefault();
                        std::string name = level["name"].asString().unwrapOrDefault();
                        int placement = level["placement"].asInt().unwrapOrDefault();
                        std::string videoURL = level["video_url"].asString().unwrapOrDefault();
                        std::optional<int> percent =
                            withPercent
                            ? std::make_optional(level["percent"].asInt().unwrapOrDefault())
                            : std::nullopt;

                        list.push_back({id, name, placement, videoURL, percent});
                    }
                };

                parseList(data["levels"]["main"], globalListCountryAdvanced.mainList);
                parseList(data["levels"]["extended"], globalListCountryAdvanced.extendedList);
                parseList(data["levels"]["advanced"], globalListCountryAdvanced.advancedList);
                parseList(data["levels"]["unbounded"], globalListCountryAdvanced.unboundedList);
                parseList(data["levels"]["progress"], globalListCountryAdvanced.progressList, true);
                parseList(data["levels"]["verified"], globalListCountryAdvanced.verifiedList);
                parseList(data["levels"]["uncompleted"], globalListCountryAdvanced.uncompletedList);

                int userCount = data["user_count"].asInt().unwrapOrDefault();
                globalListCountryAdvanced.userCount = userCount;
            }
        );
    }
};