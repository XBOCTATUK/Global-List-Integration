#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../API/API.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

inline std::string toParam(matjson::Value const& value) {
    if (value.isString()) return value.asString().unwrap();
    if (value.isNumber()) return value.dump();
    if (value.isBool()) return value.asBool().unwrap() ? "true" : "false";

    return value.dump();
}

inline APIMessage getAPIMessage(std::string msg) {
    if (msg == "too_many_requests") return APIMessage::TooManyRequests;
    if (msg == "unauthorized") return APIMessage::Unauthorized;
    if (msg == "invalid_login_or_password") return APIMessage::InvalidLoginOrPassword;
    if (msg == "invalid_verification_token_or_secret_code") return APIMessage::InvalidVerifTokenOrSecretCode;
    if (msg == "country_not_found") return APIMessage::CountryNotFound;
    if (msg == "level_not_found") return APIMessage::LevelNotFound;
    if (msg == "user_not_found") return APIMessage::UserNotFound;
    if (msg == "invalid_level_parameters") return APIMessage::InvalidLevelParameters;

    return APIMessage::Unknown;
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
            if (!res.ok()) {
                if (res.code() == -1) {
                    log::error("Failed to load data from endpoint '{}'. HTTP Error w/o message.", url);
                    cb(matjson::Value::object(), {APIErrorType::HTTPError, APIMessage::None});
                }
                else {
                    auto wrappedJSON = res.json();
                    if (!wrappedJSON) {
                        log::error("Failed to load data from endpoint '{}'. HTTP Error, failed to parse json", url);
                        cb(matjson::Value::object(), {APIErrorType::JSONError, APIMessage::None});
                    }

                    auto json = wrappedJSON.unwrap();
                    if (!json.contains("message") || !json["message"].isString()) {
                        log::error("Failed to load data from endpoint '{}'. HTTP Error, failed to receive message.", url);
                        cb(matjson::Value::object(), {APIErrorType::JSONError, APIMessage::None});
                    }

                    auto message = json["message"].asString().unwrapOrDefault();
                    log::error("Failed to load data from endpoint '{}'. HTTP Error, message: {}.", url, message);
                    cb(matjson::Value::object(), {APIErrorType::HTTPError, getAPIMessage(message)});
                }
                return;
            }

            auto wrappedJSON = res.json();
            if (!wrappedJSON) {
                log::error("Failed to parse data from endpoint '{}'.", url);
                cb(matjson::Value::object(), {APIErrorType::JSONError, APIMessage::None});
                return;
            }

            auto json = wrappedJSON.unwrap();
            if (!json.contains("data") || !json["data"].isObject()) {
                log::error("The server returned an invalid response while loading data from endpoint '{}'.", url);
                cb(matjson::Value::object(), {APIErrorType::InvalidAPIResponse, APIMessage::None});
                return;
            }

            auto data = json["data"];

            cb(std::move(data), {});
        });
    }
}