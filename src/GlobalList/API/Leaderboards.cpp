#include "Leaderboards.hpp"

namespace GlobalList::API {
    extern std::string API_URL;

    void getPlayerLeaderboard(size_t page) {
        spawn(web::WebRequest().get(fmt::format("{}/leaderboard/user/list?limit=50&offset={}", API_URL, 50 * page)),
            [](web::WebResponse value) {
                if (!value.ok()) {
                    log::error("Failed to load GlobalList leaderboards. Failed code: {}", value.code());
                    Utils::failure(value.code());
                    return;
                }
                else {
                    
                }
            }
        );
    }

    void getCountryLeaderboard() {

    }

    void getCountryAltLeaderboard() {

    }
};