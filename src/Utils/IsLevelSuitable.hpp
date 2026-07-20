#include "../Filters/Filters.hpp"
#include "../Cache/Users/Users.hpp"
#include "../Cache/GameLevels/GameLevels.hpp"
#include "../Models/GDLLevel.hpp"
#include <ranges>

namespace Utils {
    inline bool isLevelSuitable(const GDLLevel* level) {
        auto& levelFilters = GDL::Filters::getLevelFilters();
        if (levelFilters.isDefault()) return true;

        DifficultyFilter levelDiff;
        if (level->placement <= 75) levelDiff = DifficultyFilter::Top75;
        else if (level->placement <= 150) levelDiff = DifficultyFilter::Top150;
        else if (level->placement <= 300) levelDiff = DifficultyFilter::Top300;
        else if (level->placement > 300) levelDiff = DifficultyFilter::Unbounded;
        if (
            levelFilters.diffFilter == DifficultyFilter::Custom &&
            level->placement >= levelFilters.customDiffFilter[0] &&
            level->placement <= levelFilters.customDiffFilter[1]
        ) levelDiff = DifficultyFilter::Custom;

        LengthFilter levelLength;
        if (level->length < 30) levelLength = LengthFilter::Short;
        else if (level->length < 60) levelLength = LengthFilter::Medium;
        else if (level->length < 120) levelLength = LengthFilter::Long;
        else if (level->length >= 120) levelLength = LengthFilter::XL;
        if (
            levelFilters.lengthFilter == LengthFilter::Custom &&
            level->length >= levelFilters.customLengthFilter[0] &&
            level->length <= levelFilters.customLengthFilter[1]
        ) levelLength = LengthFilter::Custom;

        
        bool levelIsCompleted = false;
        if (auto user = GDL::Cache::Users::getUser(levelFilters.userID)) {
            auto& completedList = user->getCompletedList();

            levelIsCompleted = std::ranges::find_if(
                completedList,
                [internalID = level->id](const GDLBasicLevel& record) {
                    return record.id == internalID;
                }
            ) != completedList.end();
        }

        auto gameLevel = GDL::Cache::GameLevels::getGameLevel(level->ingameID);

        
        bool byDifficulty = levelFilters.diffFilter == levelDiff || levelFilters.diffFilter == DifficultyFilter::None;
        bool byLength = levelFilters.lengthFilter == levelLength || levelFilters.lengthFilter == LengthFilter::None;
        
        bool byRate =
        levelFilters.rated || levelFilters.unrated ?
            gameLevel ?
                levelFilters.rated ? gameLevel->rated : !gameLevel->rated
            : false
        : true;

        bool byPlayer = levelFilters.completedBy ? levelIsCompleted : true;

        bool byCreator =
        levelFilters.createdBy ?
            gameLevel ?
                levelFilters.createdBy ? gameLevel->creatorName == levelFilters.holder : true
            : false
        : true;

        return byDifficulty && byLength && byRate && byPlayer && byCreator;
    }
};