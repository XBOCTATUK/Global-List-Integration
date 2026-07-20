#pragma once

#include <string>

enum class APIErrorType {
    None, HTTPError, JSONError,
    InvalidAPIResponse, NoSearchResults, InvalidEndpointResponse
};

enum class APIMessage {
    None, Unknown, Success, InvalidLoginOrPassword, InvalidVerifTokenOrSecretCode,
    CountryNotFound, LevelNotFound, InvalidLevelParameters, UserNotFound, TooManyRequests, Unauthorized
};

struct APIError {
    APIErrorType type = APIErrorType::None;
    APIMessage message = APIMessage::Success;

    explicit operator bool() const {
        return type != APIErrorType::None || message != APIMessage::Success;
    }

    constexpr std::string_view typeAsString() {
        switch (type) {
            case APIErrorType::None: return "None";
            case APIErrorType::HTTPError: return "HTTP Error";
            case APIErrorType::JSONError: return "JSON Error";
            case APIErrorType::InvalidAPIResponse: return "Invalid API Response";
            case APIErrorType::NoSearchResults: return "No Search Results";
            case APIErrorType::InvalidEndpointResponse: return "Invalid Endpoint Response";
        }

        return "Unknown API Error Type";
    }

    constexpr std::string_view messageAsString() {
        switch (message) {
            case APIMessage::None: return "None";
            case APIMessage::Unknown: return "Unknown";
            case APIMessage::Success: return "Success";
            case APIMessage::InvalidLoginOrPassword: return "Invalid Login Or Password";
            case APIMessage::InvalidVerifTokenOrSecretCode: return "Invalid Verification Token Or Secret Code";
            case APIMessage::CountryNotFound: return "Country Not Found";
            case APIMessage::LevelNotFound: return "Level Not Found";
            case APIMessage::InvalidLevelParameters: return "Invalid Level Parameters";
            case APIMessage::UserNotFound: return "User Not Found";
            case APIMessage::TooManyRequests: return "Too Many Requests";
            case APIMessage::Unauthorized: return "Unauthorized";
        }

        return "Unknown API Message";
    }
};