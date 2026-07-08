#pragma once

enum class APIErrorType {
    None, HTTPError, JSONError, InvalidAPIResponse, InvalidEndpointResponse
};

enum class APIMessage {
    None, Unknown, Success, InvalidLoginOrPassword, InvalidVerifTokenOrSecretCode, CountryNotFound, LevelNotFound, InvalidLevelParameters, UserNotFound, TooManyRequests, Unauthorized
};

struct APIError {
    APIErrorType type = APIErrorType::None;
    APIMessage message = APIMessage::Success;

    explicit operator bool() const {
        return type != APIErrorType::None || message != APIMessage::Success;
    }
};