#include "Levels.hpp"

namespace GlobalList::Levels {
    static std::unordered_map<int, GlobalListLevel> globalListLevels;
    static std::vector<int> keys;

    void saveLevel(int levelID, const GlobalListLevel& levelData) {
        auto it = globalListLevels.find(levelID);

        if (it == globalListLevels.end()) {
            keys.push_back(levelID);
        }

        globalListLevels[levelID] = levelData;
    }

    GlobalListLevel* getLevelByID(int levelID) {
        if (globalListLevels.find(levelID) != globalListLevels.end())
            return &globalListLevels[levelID];

        return nullptr;
    }

    GlobalListLevel* getLevelByIndex(size_t idx) {
        if (idx >= keys.size()) return nullptr;

        int levelID = keys[idx];
        return &globalListLevels[levelID];
    }

    bool isSuitable(GlobalListLevel* level) {
        auto& levelFilters = GlobalList::Filters::getLevelFilters();
        if (levelFilters.isDefault()) return true;

        int levelLength = 0;
        if (level->length < 30) levelLength = 0;
        else if (level->length < 60) levelLength = 1;
        else if (level->length < 120) levelLength = 2;
        else if (level->length >= 120) levelLength = 3;
        if (levelFilters.lengthFilter[4] && (levelFilters.customLengthFilter[0] != 0 ? level->length >= levelFilters.customLengthFilter[0] : true) && (levelFilters.customLengthFilter[1] != 0 ? level->length <= levelFilters.customLengthFilter[1] : true)) levelLength = 4;

        int levelDiff = 0;
        if (level->placement <= 50) levelDiff = 0;
        else if (level->placement <= 150) levelDiff = 1;
        else if (level->placement <= 300) levelDiff = 2;
        else if (level->placement > 300) levelDiff = 3;
        else if (levelFilters.diffFilter[4] && (levelFilters.customDiffFilter[0] != 0 ? level->placement >= levelFilters.customDiffFilter[0] : true) && (levelFilters.customDiffFilter[1] != 0 ? level->placement < levelFilters.customDiffFilter[1] : true)) levelDiff = 4;

        auto levelData = GlobalList::Cache::getLevelData(level->levelID);
        auto userData = GlobalList::Cache::getUser(levelFilters.username);
        bool levelIsCompleted = std::ranges::find_if(
            userData->records,
            [levelID = level->levelID](const UserLevel& userLevel) {
                return userLevel.levelID == levelID;
            }
        ) != userData->records.end();
        
        bool byLength = levelFilters.lengthFilter[levelLength] || levelFilters.lengthFilter == LevelFilters{}.lengthFilter;
        bool byDifficulty = levelFilters.diffFilter[levelDiff] || levelFilters.diffFilter == LevelFilters{}.diffFilter;
        bool byRate = levelFilters.rated ? levelData->rated : (levelFilters.unrated ? levelData->unrated : true);
        bool byPlayer = levelFilters.completed ? ((userData && userData->records.size() != 0) ? levelIsCompleted : false) : true;
        bool byCreator = (levelFilters.byHolder ? levelData->holder == levelFilters.holder : true);

        return byLength && byDifficulty && byRate && byPlayer && byCreator;
    }

    size_t size() {
        return keys.size();
    }

    void clear() {
        globalListLevels.clear();
        keys.clear();
    }
};