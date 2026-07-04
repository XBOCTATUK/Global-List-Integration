#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../API/API.hpp"
#include "Failure.hpp"

using namespace geode::prelude;

inline std::string toParam(matjson::Value const& value) {
    if (value.isString())
        return value.asString().unwrap();
    if (value.isNumber())
        return value.dump();
    if (value.isBool())
        return value.asBool().unwrap() ? "true" : "false";

    return value.dump();
}

inline std::string getEndPointName(const std::string& endPoint) {
    if (endPoint == GlobalList::API::loginEP) return "verification";
    else if (endPoint == GlobalList::API::verifyLoginEP) return "JWT";
    else if (endPoint == GlobalList::API::levelEP) return "level";
    else if (endPoint == GlobalList::API::levelListEP) return "demonlist";
    else if (endPoint == GlobalList::API::userEP) return "user";
    else if (endPoint == GlobalList::API::userRecordsEP) return "user records";
    else if (endPoint == GlobalList::API::userLeaderboardEP) return "user leaderboard";
    else if (endPoint == GlobalList::API::countryLeaderboardEP) return "country leaderboard";
    else if (endPoint == GlobalList::API::getMainCountryLeaderboardEP) return "main country leaderboard";
    else if (endPoint == GlobalList::API::getAdvanvedCountryLeaderboardEP) return "advanced country leaderboard";
    else return "some";
}

namespace Utils {
    template <typename Callback>
    arc::TaskHandle<void> WebReq(std::string const& url, matjson::Value const& params, matjson::Value const& bodyJSON, Callback&& cb) {
        web::WebRequest req;

        if (params.isObject() && params.size() != 0) {
            for (auto const& [key, value] : params)
                req.param(key, toParam(value));
        }
        if (bodyJSON.isObject() && bodyJSON.size() != 0) {
            req.bodyJSON(bodyJSON);
        }

        return geode::async::spawn(req.get(url), [cb = std::forward<Callback>(cb), url](web::WebResponse res) mutable {
            auto whatsFailed = getEndPointName(url);

            if (!res.ok()) {
                Utils::failure(
                    "HTTP Req Error",
                    fmt::format("Failed to load {} data. Please try again later.", whatsFailed).c_str()
                );
                return;
            }

            auto wrappedJSON = res.json();
            if (!wrappedJSON) {
                Utils::failure(
                    "Unwrap JSON Error",
                    fmt::format("Failed to parse {} data. Please try again later.", whatsFailed).c_str()
                );
                return;
            }

            auto json = wrappedJSON.unwrap();
            if (!json.contains("data") || !json["data"].isObject() || json["data"].size() == 0) {
                Utils::failure(
                    "Invalid Response",
                    fmt::format("The server returned an invalid response while loading {} data. Please try again later.", whatsFailed).c_str()
                );
                return;
            }

            auto data = json["data"];

            cb(std::move(data));
        });
    }
}