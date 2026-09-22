#pragma once

inline const std::unordered_map<std::string, std::string> badgeNames = {
    {"pc", "pc.png"_spr},
    {"mobile", "mobile.png"_spr},
    {"contributor", "contributor.png"_spr},
    {"former_cheater", "former_cheater.png"_spr}
};

inline const std::unordered_map<std::string, std::string> badgeInfo = {
    {"pc", "Player who primarily completes levels on PC."},
    {"mobile", "Player who primarily completes levels on mobile."},
    {"contributor", "Player who has contributed to Demonlist through financial support or other valuable contributions."},
    {"former_cheater", "Player who was previously banned from Demonlist for cheating."}
};

namespace Utils {
    inline std::string getBadgeSpriteName(const std::string& badgeName) {
        auto it = badgeNames.find(badgeName);
        if (it == badgeNames.end()) return "pc.png"_spr;

        return it->second;
    }

    inline std::string getBadgeInfo(const std::string& badgeName) {
        auto it = badgeInfo.find(badgeName);
        if (it == badgeInfo.end()) return badgeInfo.at("pc");

        return it->second;
    }
}