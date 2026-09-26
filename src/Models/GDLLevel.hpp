#pragma once

struct GDLLevel {
    int id = 0;
    int ingameID = 0;
    int placement = 0;
    std::string name;
    double points = 0.0;
    int listPercent = 0;
    int length = 0;
    std::string holder;
    std::string verifier;
    int verifierID = 0;
    std::string verificationURL;
    std::string dateCreated;
    
    std::optional<int> objects;
    std::optional<std::string> description;
    std::optional<std::string> creator;
    std::optional<std::string> songURL;
    std::optional<int> gameVersion;
    std::optional<bool> isCopyable;
    std::optional<std::string> password;

    bool isFull() const {
        return objects.has_value();
    }

    bool contains(const std::string& query) const {
        std::string lowerName = name;
        std::string lowerQuery = query;

        std::transform(
            lowerName.begin(), lowerName.end(), lowerName.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        std::transform(
            lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );
        
        return 
            lowerName.find(lowerQuery) != std::string::npos ||
            std::to_string(ingameID) == lowerQuery;
    }
};