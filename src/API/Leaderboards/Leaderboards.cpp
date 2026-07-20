#include "Leaderboards.hpp"

namespace GDL::API::Leaderboards {
    void getUserLeaderboard(int page, const std::string& search, const std::string& country) {
        auto key = UserLeaderboardKey{page, search, country};
        auto cachedUsers = GDL::Cache::Leaderboards::getUserLeaderboard(key);
        if (cachedUsers) {
            UserLeaderboardLoadedEvent().send(
                Ok(cachedUsers)
            );
            return;
        }

        Utils::WebReq(
            USER_LEADERBOARD_EP,
            matjson::makeObject({ {"limit", 50}, {"offset", 50 * (page-1)}, {"search", search}, {"country", country} }),
            matjson::Value::object(),
            [key](matjson::Value data, APIError error) {
                if (error) {
                    UserLeaderboardLoadedEvent().send(
                        Err(error)
                    );
                    return;
                }
                else if (!data.contains("users") || !data["users"].isArray()) {
                    log::error("The user leaderboard data is incomplete or invalid.");
                    UserLeaderboardLoadedEvent().send(
                        Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None})
                    );
                    return;
                }
                else if (data["users"].size() == 0) {
                    log::error("The user leaderboard data is empty (no search results).");
                    UserLeaderboardLoadedEvent().send(
                        Err(APIError{APIErrorType::NoSearchResults, APIMessage::None})
                    );
                    return;
                }

                std::vector<int> userIDs;

                for (const auto& user : data["users"]) {
                    int id = user["id"].asInt().unwrapOrDefault();
                    std::string username = user["username"].asString().unwrapOrDefault();
                    int placement = user["placement"].asInt().unwrapOrDefault();
                    double points = user["points"].asDouble().unwrapOrDefault();
                    std::string country = user["country"].asString().unwrapOrDefault();
                    std::string badge = user["badge"].asString().unwrapOrDefault();

                    auto gdlUser = GDLUser{id, username, placement, points, country, badge};

                    GDL::Cache::Users::setUser(std::move(gdlUser));
                    userIDs.push_back(id);
                }
                
                GDL::Cache::Leaderboards::setUserLeaderboard(key, std::move(userIDs));
                UserLeaderboardLoadedEvent().send(
                    Ok(GDL::Cache::Leaderboards::getUserLeaderboard(key))
                );
            }
        );
    }

    void getCountryLeaderboard(CountriesLeaderboardType type) {
        auto cachedCountry = GDL::Cache::Leaderboards::getCountryLeaderboard(type);
        if (cachedCountry) {
            CountryLeaderboardLoadedEvent(type).send(
                Ok(cachedCountry)
            );
            return;
        }

        auto typeStr = type == CountriesLeaderboardType::Main ? "main" : "advanced";

        Utils::WebReq(
            COUNTRY_LEADERBOARD_EP,
            matjson::makeObject({ {"type", typeStr} }),
            matjson::Value::object(),
            [type](matjson::Value data, APIError error) {
                if (error) {
                    CountryLeaderboardLoadedEvent(type).send(
                        Err(error)
                    );
                    return;
                }
                else if (!data.contains("countries") || !data["countries"].isArray() || data["countries"].size() == 0) {
                    log::error("The countries leaderboard data is incomplete or invalid.");
                    CountryLeaderboardLoadedEvent(type).send(
                        Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None})
                    );
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
                
                GDL::Cache::Leaderboards::setCountryLeaderboard(type, std::move(countries));
                CountryLeaderboardLoadedEvent(type).send(
                    Ok(GDL::Cache::Leaderboards::getCountryLeaderboard(type))
                );
            }
        );
    }

    void getMainCountryLeaderboard(const std::string& country) {
        auto cachedCountryUsers = GDL::Cache::Leaderboards::getMainCountryLeaderboard(country);
        if (cachedCountryUsers) {
            MainCountryLeaderboardLoadedEvent(country).send(
                Ok(cachedCountryUsers)
            );
            return;
        }

        Utils::WebReq(
            MAIN_COUNTRY_LEADERBOARD_EP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [country](matjson::Value data, APIError error) {
                if (error) {
                    MainCountryLeaderboardLoadedEvent(country).send(
                        Err(error)
                    );
                    return;
                }
                else if (!data.contains("users") || !data["users"].isArray() || data["users"].size() == 0) {
                    log::error("The main country leaderboard data is incomplete or invalid.");
                    MainCountryLeaderboardLoadedEvent(country).send(
                        Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None})
                    );
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
                
                GDL::Cache::Leaderboards::setMainCountryLeaderboard(country, std::move(countryUsers));
                MainCountryLeaderboardLoadedEvent(country).send(
                    Ok(GDL::Cache::Leaderboards::getMainCountryLeaderboard(country))
                );
            }
        );
    }

    void getAdvancedCountryLeaderboard(const std::string& country) {
        auto cachedCountryAdvanced = GDL::Cache::Leaderboards::getAdvancedCountryLeaderboard(country);
        if (cachedCountryAdvanced) {
            AdvancedCountryLeaderboardLoadedEvent(country).send(
                Ok(cachedCountryAdvanced)
            );
            return;
        }

        Utils::WebReq(
            ADVANCED_COUNTRY_LEADERBOARD_EP,
            matjson::makeObject({ {"country", country} }),
            matjson::Value::object(),
            [country](matjson::Value data, APIError error) {
                if (error) {
                    UserLeaderboardLoadedEvent().send(
                        Err(error)
                    );
                    return;
                }
                else if (!data.contains("levels") || !data["levels"].isObject() || data["levels"].size() == 0) {
                    log::error("The advanced country leaderboard data is incomplete or invalid.");
                    UserLeaderboardLoadedEvent().send(
                        Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None})
                    );
                    return;
                }
                
                GDLCountryAdvanced gdlCountryAdvanced;

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

                int userCount = data["user_count"].asInt().unwrapOrDefault();
                gdlCountryAdvanced.userCount = userCount;
                
                GDL::Cache::Leaderboards::setAdvancedCountryLeaderboard(country, std::move(gdlCountryAdvanced));
                AdvancedCountryLeaderboardLoadedEvent(country).send(
                    Ok(GDL::Cache::Leaderboards::getAdvancedCountryLeaderboard(country))
                );
            }
        );
    }
};