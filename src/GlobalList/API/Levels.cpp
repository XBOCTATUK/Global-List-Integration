#include "Levels.hpp"

namespace GlobalList::API {
    std::string API_URL = "https://api.demonlist.org";

    void Utils::failure(int code);

    void getDemonlist() {
        spawn(web::WebRequest().get(fmt::format("{}/level/classic/list", API_URL)),
            [](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to load GlobalList. Failed code: {}", value.code());
                    Utils::failure(value.code());
                    return;
                }

                GlobalList::Levels::clear();
                auto data = value.json();

                matjson::Value json = data.ok().value();
                if (!json.contains("data") || !json["data"].contains("levels") || !json["data"]["levels"].isArray() || json["data"]["levels"].size() == 0) {
                    Utils::failure(204);
                    return;
                }

                for (const auto& level : json["data"]["levels"]) {
                    int id = level["id"].asInt().unwrapOr(0);
                    int levelID = level["ingame_id"].asInt().unwrapOr(0);
                    std::string name = level["name"].asString().unwrapOr("");
                    int placement = level["placement"].asInt().unwrapOr(0);
                    int length = level["length"].asInt().unwrapOr(0);
                    GlobalListLevel newLevel = { id, levelID, name, placement, length };

                    if (newLevel.levelID == 0 || newLevel.name == "" || newLevel.placement == 0) {
                        Utils::failure(204);
                        return;
                    }

                    GlobalList::Cache::savePlacement(levelID, placement);
                    GlobalList::Levels::saveLevel(levelID, newLevel);
                }

                PopulateListEvent().send();
            }
        );
    }

    void getLevelPlacement(int levelID) {
        int cachedPlacement = GlobalList::Cache::getPlacement(levelID);
        if (cachedPlacement != -1) {
            PositionEvent(levelID).send(cachedPlacement);
        }
        else {
            spawn(web::WebRequest().get(fmt::format("{}/level/classic/get?ingame_id={}", API_URL, levelID)),
                [levelID](web::WebResponse value) {
                    int placement = GlobalList::Cache::getPlacement(levelID, false);
                    if (!value.ok()) {
                        if (value.code() == 404 && placement == -1) placement = 0;
                    }
                    else {
                        auto data = value.json();
                        auto json = data.ok().value();

                        auto levelData = json["data"];

                        placement = levelData["placement"].asInt().ok().value();

                        if (placement > 0) GlobalList::Cache::savePlacement(levelID, placement);
                        else placement = -1;
                    }

                    PositionEvent(levelID).send(placement);
                }
            );
        }
    }
};