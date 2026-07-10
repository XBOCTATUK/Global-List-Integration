#include "../Filters/Filters.hpp"
#include "../Cache/Users/Users.hpp"
#include "../Cache/GameLevels/GameLevels.hpp"
#include "../Models/GDLLevel.hpp"
#include <ranges>

namespace Utils {
    inline bool isLevelSuitable(GDLLevel* level) {
        auto& levelFilters = GDL::Filters::getLevelFilters();
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
        if (levelFilters.diffFilter[4] && (levelFilters.customDiffFilter[0] != 0 ? level->placement >= levelFilters.customDiffFilter[0] : true) && (levelFilters.customDiffFilter[1] != 0 ? level->placement < levelFilters.customDiffFilter[1] : true)) levelDiff = 4;

        auto gameLevel = GDL::Cache::GameLevels::getGameLevel(level->ingameID);
        auto userData = GDL::Cache::Users::getUserRecords(levelFilters.userID);
        bool levelIsCompleted = userData ? std::ranges::find_if(
            userData->records,
            [internalID = level->id](const GDLRecord& record) {
                return record.internalID == internalID;
            }
        ) != userData->records.end() : false;
        
        bool byLength = levelFilters.lengthFilter[levelLength] || levelFilters.lengthFilter == LevelFilters{}.lengthFilter;
        bool byDifficulty = levelFilters.diffFilter[levelDiff] || levelFilters.diffFilter == LevelFilters{}.diffFilter;
        bool byRate =
        gameLevel ?
            levelFilters.rated ? gameLevel->rated
            : levelFilters.unrated ? !gameLevel->rated
            : true
        : false;
        bool byCreator = gameLevel ? (levelFilters.byHolder ? gameLevel->creatorName == levelFilters.holder : true) : false;
        bool byPlayer = levelFilters.completed ? ((userData && userData->records.size() != 0) ? levelIsCompleted : false) : true;

        return byLength && byDifficulty && byRate && byPlayer && byCreator;
    }
};