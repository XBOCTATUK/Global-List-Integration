#include "Leaderboards.hpp"

namespace GlobalList::API {
    void getUserLeaderboard(int page, std::string search, std::string country) {
        auto cachedUsers = GlobalList::Cache::Leaderboards::getUsers(page);
        if (!cachedUsers.empty()) {
            UserLeaderboardLoadedEvent().send(cachedUsers);
            return;
        }

        Utils::WebReq(
            userLeaderboardEP,
            matjson::makeObject({ {"offset", 50 * (page-1)}, {"search", search}, {"country", country} }),
            matjson::Value::object(),
            [page](matjson::Value data) {
                if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The player leaderboard data is incomplete or invalid. Please try again later."
                    );
                }

                std::vector<GlobalListUser> users;

                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    int placement = user["placement"].asInt().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();
                    std::string country = user["country"].asString().unwrapOrDefault();
                    std::string badge = user["badge"].asString().unwrapOrDefault();

                    auto GDLUser = GlobalListUser{id, username, placement, points, country, badge};
                    users.push_back(GDLUser);
                }
                
                GlobalList::Cache::Leaderboards::setUsers(std::move(users));
                UserLeaderboardLoadedEvent().send(GlobalList::Cache::Leaderboards::getUsers(page));
            }
        );
    }

    void getCountryLeaderboard(CountriesLeaderboardType type) {
        auto cachedCountry = GlobalList::Cache::Leaderboards::getCountry(type);
        if (cachedCountry) {
            CountryLeaderboardLoadedEvent(type).send(cachedCountry);
            return;
        }

        auto typeStr = type == CountriesLeaderboardType::Main ? "main" : "advanced";

        Utils::WebReq(
            countryLeaderboardEP,
            matjson::makeObject({ {"type", typeStr} }),
            matjson::Value::object(),
            [type](matjson::Value data) {
                if (!data.contains("countries") || !data["countries"].isArray() || data["countries"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The countries leaderboard data is incomplete or invalid. Please try again later."
                    );
                }

                std::vector<GlobalListCountry> countries;

                for (const auto& country : data["countries"]) {
                    std::string title = country["title"].asString().unwrapOrDefault();
                    int placement = country["placement"].asInt().unwrapOrDefault();
                    double points = country["points"].asDouble().unwrapOrDefault();

                    auto GDLCountry = GlobalListCountry{title, placement, points};
                    countries.push_back(GDLCountry);
                }
                
                GlobalList::Cache::Leaderboards::setCountries(type, std::move(countries));
                CountryLeaderboardLoadedEvent(type).send(GlobalList::Cache::Leaderboards::getCountry(type));
            }
        );
    }

    void getMainCountryLeaderboard(std::string country) {
        auto cachedCountryUsers = GlobalList::Cache::Leaderboards::getCountryUsers(country);
        if (cachedCountryUsers) {
            MainCountryLeaderboardLoadedEvent(country).send(cachedCountryUsers);
            return;
        }

        Utils::WebReq(
            getMainCountryLeaderboardEP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [country](matjson::Value data) {
                if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The main country leaderboard data is incomplete or invalid. Please try again later."
                    );
                }

                std::vector<GlobalListCountryUser> countryUsers;
                
                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();

                    auto globalListCountryUser = GlobalListCountryUser{id, username, points};
                    countryUsers.push_back(globalListCountryUser);
                }
                
                GlobalList::Cache::Leaderboards::setCountryUsers(country, std::move(countryUsers));
                MainCountryLeaderboardLoadedEvent(country).send(GlobalList::Cache::Leaderboards::getCountryUsers(country));
            }
        );
    }

    void getAdvancedCountryLeaderboard(std::string country) {
        auto cachedCountryAdvanced = GlobalList::Cache::Leaderboards::getCountryAdvanced(country);
        if (cachedCountryAdvanced) {
            AdvancedCountryLeaderboardLoadedEvent(country).send(cachedCountryAdvanced);
            return;
        }

        Utils::WebReq(
            getAdvanvedCountryLeaderboardEP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [country](matjson::Value data) {
                if (!data.contains("levels") || !data["levels"].isObject() || data["levels"].size() == 0) {
                    Utils::failure(
                        "Invalid Response",
                        "The advanced country leaderboard data is incomplete or invalid. Please try again later."
                    );
                }
                
                auto globalListCountryAdvanced = GlobalListCountryAdvanced{};

                int hardestID = data["levels"]["hardest"]["id"].asInt().unwrapOrDefault();
                std::string hardestName = data["levels"]["hardest"]["name"].asString().unwrapOrDefault();
                int hardestPlacement = data["levels"]["hardest"]["placement"].asInt().unwrapOrDefault();
                std::string hardestVideoURL = data["levels"]["hardest"]["video_url"].asString().unwrapOrDefault();

                globalListCountryAdvanced.hardestLevel = {hardestID, hardestName, hardestPlacement, hardestVideoURL};

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
                
                GlobalList::Cache::Leaderboards::setCountryAdvanced(country, std::move(globalListCountryAdvanced));
                AdvancedCountryLeaderboardLoadedEvent(country).send(GlobalList::Cache::Leaderboards::getCountryAdvanced(country));
            }
        );
    }
};