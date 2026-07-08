#include "Leaderboards.hpp"

namespace GDL::API {
    void getUserLeaderboard(int page, std::string search, std::string country) {
        auto cachedUsers = GDL::Cache::Leaderboards::getUsers(page);
        if (!cachedUsers.empty()) {
            UserLeaderboardLoadedEvent().send(Ok(cachedUsers));
            return;
        }

        Utils::WebReq(
            userLeaderboardEP,
            matjson::makeObject({ {"offset", 50 * (page-1)}, {"search", search}, {"country", country} }),
            matjson::Value::object(),
            [page](matjson::Value data, APIError error) {
                if (data.size() == 0 && error) {
                    UserLeaderboardLoadedEvent().send(Err(error));
                    return;
                }
                else if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    log::error("The player leaderboard data is incomplete or invalid.");
                    UserLeaderboardLoadedEvent().send(Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None}));
                    return;
                }

                std::vector<GDLUser> users;

                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    int placement = user["placement"].asInt().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();
                    std::string country = user["country"].asString().unwrapOrDefault();
                    std::string badge = user["badge"].asString().unwrapOrDefault();

                    auto gdlUser = GDLUser{id, username, placement, points, country, badge};
                    users.push_back(gdlUser);
                }
                
                GDL::Cache::Leaderboards::setUsers(std::move(users));
                UserLeaderboardLoadedEvent().send(Ok(GDL::Cache::Leaderboards::getUsers(page)));
            }
        );
    }

    void getCountryLeaderboard(CountriesLeaderboardType type) {
        auto cachedCountry = GDL::Cache::Leaderboards::getCountry(type);
        if (cachedCountry) {
            CountryLeaderboardLoadedEvent(type).send(Ok(cachedCountry));
            return;
        }

        auto typeStr = type == CountriesLeaderboardType::Main ? "main" : "advanced";

        Utils::WebReq(
            countryLeaderboardEP,
            matjson::makeObject({ {"type", typeStr} }),
            matjson::Value::object(),
            [type](matjson::Value data, APIError error) {
                if (data.size() == 0 && error) {
                    CountryLeaderboardLoadedEvent(type).send(Err(error));
                    return;
                }
                else if (!data.contains("countries") || !data["countries"].isArray() || data["countries"].size() == 0) {
                    log::error("The countries leaderboard data is incomplete or invalid.");
                    CountryLeaderboardLoadedEvent(type).send(Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None}));
                    return;
                }

                std::vector<GDLCountry> countries;

                for (const auto& country : data["countries"]) {
                    std::string title = country["title"].asString().unwrapOrDefault();
                    int placement = country["placement"].asInt().unwrapOrDefault();
                    double points = country["points"].asDouble().unwrapOrDefault();

                    auto gdlCountry = GDLCountry{title, placement, points};
                    countries.push_back(gdlCountry);
                }
                
                GDL::Cache::Leaderboards::setCountries(type, std::move(countries));
                CountryLeaderboardLoadedEvent(type).send(Ok(GDL::Cache::Leaderboards::getCountry(type)));
            }
        );
    }

    void getMainCountryLeaderboard(std::string country) {
        auto cachedCountryUsers = GDL::Cache::Leaderboards::getCountryUsers(country);
        if (cachedCountryUsers) {
            MainCountryLeaderboardLoadedEvent(country).send(Ok(cachedCountryUsers));
            return;
        }

        Utils::WebReq(
            getMainCountryLeaderboardEP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [country](matjson::Value data, APIError error) {
                if (data.size() == 0 && error) {
                    MainCountryLeaderboardLoadedEvent(country).send(Err(error));
                    return;
                }
                else if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    log::error("The main country leaderboard data is incomplete or invalid.");
                    MainCountryLeaderboardLoadedEvent(country).send(Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None}));
                    return;
                }

                std::vector<GDLCountryUser> countryUsers;
                
                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();

                    auto gdlCountryUser = GDLCountryUser{id, username, points};
                    countryUsers.push_back(gdlCountryUser);
                }
                
                GDL::Cache::Leaderboards::setCountryUsers(country, std::move(countryUsers));
                MainCountryLeaderboardLoadedEvent(country).send(Ok(GDL::Cache::Leaderboards::getCountryUsers(country)));
            }
        );
    }

    void getAdvancedCountryLeaderboard(std::string country) {
        auto cachedCountryAdvanced = GDL::Cache::Leaderboards::getCountryAdvanced(country);
        if (cachedCountryAdvanced) {
            AdvancedCountryLeaderboardLoadedEvent(country).send(Ok(cachedCountryAdvanced));
            return;
        }

        Utils::WebReq(
            getAdvanvedCountryLeaderboardEP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [country](matjson::Value data, APIError error) {
                if (data.size() == 0 && error) {
                    UserLeaderboardLoadedEvent().send(Err(error));
                    return;
                }
                else if (!data.contains("levels") || !data["levels"].isObject() || data["levels"].size() == 0) {
                    log::error("The advanced country leaderboard data is incomplete or invalid.");
                    UserLeaderboardLoadedEvent().send(Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None}));
                    return;
                }
                
                auto gdlCountryAdvanced = GDLCountryAdvanced{};

                int hardestID = data["levels"]["hardest"]["id"].asInt().unwrapOrDefault();
                std::string hardestName = data["levels"]["hardest"]["name"].asString().unwrapOrDefault();
                int hardestPlacement = data["levels"]["hardest"]["placement"].asInt().unwrapOrDefault();
                std::string hardestVideoURL = data["levels"]["hardest"]["video_url"].asString().unwrapOrDefault();

                gdlCountryAdvanced.hardestLevel = {hardestID, hardestName, hardestPlacement, hardestVideoURL};

                auto parseList = [](matjson::Value& levelData, std::vector<GDLBasicLevel>& list, bool withPercent = false) {
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

                parseList(data["levels"]["main"], gdlCountryAdvanced.mainList);
                parseList(data["levels"]["extended"], gdlCountryAdvanced.extendedList);
                parseList(data["levels"]["advanced"], gdlCountryAdvanced.advancedList);
                parseList(data["levels"]["unbounded"], gdlCountryAdvanced.unboundedList);
                parseList(data["levels"]["progress"], gdlCountryAdvanced.progressList, true);
                parseList(data["levels"]["verified"], gdlCountryAdvanced.verifiedList);
                parseList(data["levels"]["uncompleted"], gdlCountryAdvanced.uncompletedList);

                int userCount = data["user_count"].asInt().unwrapOrDefault();
                gdlCountryAdvanced.userCount = userCount;
                
                GDL::Cache::Leaderboards::setCountryAdvanced(country, std::move(gdlCountryAdvanced));
                AdvancedCountryLeaderboardLoadedEvent(country).send(Ok(GDL::Cache::Leaderboards::getCountryAdvanced(country)));
            }
        );
    }
};