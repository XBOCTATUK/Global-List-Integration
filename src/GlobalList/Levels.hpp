#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <cctype>

#include "Cache/LevelCache.hpp"
#include "Cache/UserCache.hpp"

#include "Filters.hpp"

class GlobalListLevel {
public:
	int id;
	int levelID;
	std::string name;
	int placement;
	int length;

	bool contains(const std::string& query) const {
		std::string lowerName = this->name;
        std::string lowerQuery = query;

        std::transform(
            lowerName.begin(),
            lowerName.end(),
            lowerName.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        std::transform(
            lowerQuery.begin(),
            lowerQuery.end(),
            lowerQuery.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );
		
		return lowerName.find(lowerQuery) != std::string::npos ||
			   std::to_string(this->levelID) == lowerQuery;
	}
};

namespace GlobalList::Levels {
    void saveLevel(int levelID, const GlobalListLevel& levelData);

    GlobalListLevel* getLevelByID(int levelID);
    GlobalListLevel* getLevelByIndex(size_t idx);

    bool isSuitable(GlobalListLevel* level);
    size_t size();
    void clear();
};