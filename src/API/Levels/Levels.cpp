#include "Levels.hpp"

namespace GDL::API {
    void getDemonlist() {
        auto cachedDemonlist = GDL::Cache::Levels::getDemonlist(1);
        if (!cachedDemonlist.empty()) {
            DemonlistLoadedEvent().send(Ok(cachedDemonlist));
            return;
        }

        Utils::WebReq(
            levelListEP,
            matjson::Value::object(),
            matjson::Value::object(),
            [](matjson::Value data, APIError error) {
                if (data.size() == 0 && error) {
                    DemonlistLoadedEvent().send(Err(error));
                    return;
                }
                else if (!data.contains("levels") || !data["levels"].isArray() || data["levels"].size() == 0) {
                    log::error("The demonlist data is incomplete or invalid.");
                    DemonlistLoadedEvent().send(Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None}));
                    return;
                }

                std::vector<GDLLevel> levels;

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

                    auto GDLLLevel = GDLLevel{
                        id, ingameID, placement, name, points,
                        listPercent, length, holder, verifier,
                        verifierID, verificationURL, dateCreated
                    };
                    levels.push_back(GDLLLevel);
                }
                
                GDL::Cache::Levels::setDemonlist(std::move(levels));
                DemonlistLoadedEvent().send(Ok(GDL::Cache::Levels::getDemonlist(1)));
            }
        );
    }

    void getLevel(int levelID) {
        auto cachedLevel = GDL::Cache::Levels::getLevel(levelID);
        if (cachedLevel) {
            LevelLoadedEvent(levelID).send(Ok(cachedLevel));
            return;
        }

        Utils::WebReq(
            levelEP,
            matjson::makeObject({ {"ingame_id", levelID} }),
            matjson::Value::object(),
            [levelID](matjson::Value data, APIError error) {
                if (data.size() == 0 && error) {
                    LevelLoadedEvent(levelID).send(Err(error));
                    return;
                }
                else if (!data.isObject() || data.size() == 0) {
                    log::error("The level data is incomplete or invalid.");
                    LevelLoadedEvent(levelID).send(Err(APIError{APIErrorType::InvalidEndpointResponse, APIMessage::None}));
                    return;
                }

                int id = data["id"].asInt().unwrapOrDefault();
                int ingameID = data["ingame_id"].asInt().unwrapOrDefault();
                int placement = data["placement"].asInt().unwrapOrDefault();
                std::string name = data["name"].asString().unwrapOrDefault();
                double points = data["points"].asDouble().unwrapOrDefault();
                int listPercent = data["list_percent"].asInt().unwrapOrDefault();
                int length = data["length"].asInt().unwrapOrDefault();
                int objects = data["objects"].asInt().unwrapOrDefault();
                std::string description = data["description"].asString().unwrapOrDefault();
                std::string creator = data["creator"].asString().unwrapOrDefault();
                std::string holder = data["holder"].asString().unwrapOrDefault();
                std::string songURL = data["song_url"].asString().unwrapOrDefault();
                int gameVersion = data["game_version"].asInt().unwrapOrDefault();
                std::string verifier = data["verification"]["username"].asString().unwrapOrDefault();
                int verifierID = data["verification"]["user_id"].asInt().unwrapOrDefault();
                std::string verificationURL = data["verification"]["video_url"].asString().unwrapOrDefault();
                bool isCopyable = data["copy_info"]["is_copyable"].asBool().unwrapOrDefault();
                std::string password = data["copy_info"]["password"].asString().unwrapOrDefault();
                std::string dateCreated = data["date_created"].asString().unwrapOrDefault();

                auto GDLLevel = GDLLevel{
                    id, ingameID, placement, name, points, listPercent,
                    length, holder, verifier, verifierID, verificationURL,
                    dateCreated, objects, description, creator,
                    songURL, gameVersion, isCopyable, password
                };
                
                GDL::Cache::Levels::setLevel(std::move(GDLLevel));
                LevelLoadedEvent(levelID).send(Ok(GDL::Cache::Levels::getLevel(levelID)));
            }
        );
    }
};