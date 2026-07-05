#pragma once

#include <string>
#include <optional>

struct GlobalListLevel {
    int id;
    int ingameID;
    int placement;
    std::string name;
    double points;
    int listPercent;
    int length;
    std::string holder;
    std::string verifier;
    int verifierID;
    std::string verificationURL;
    std::string dateCreated;
    
    std::optional<int> objects;
    std::optional<std::string> description;
    std::optional<std::string> creator;
    std::optional<std::string> songURL;
    std::optional<int> gameVersion;
    std::optional<bool> isCopyable;
    std::optional<std::string> password;
};