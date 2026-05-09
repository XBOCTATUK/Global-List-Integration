#pragma once

#include <string>
#include <unordered_map>
#include <ctime>

struct LevelData {
	bool rated;
	bool unrated;
	std::string holder;
};

namespace GlobalList::Cache {
    void saveLevelData(int levelID, const LevelData& levelData);

    LevelData* getLevelData(int levelID);

    size_t levelDataSize();
    void levelDataClear();
    void updateLevelLoadTime();
}