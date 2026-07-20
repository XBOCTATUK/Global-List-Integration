#include "Filters.hpp"

namespace GDL::Filters {
    static LevelFilters displayFilters;
    static LevelFilters levelFilters;

    const LevelFilters& getDisplayFilters() { return displayFilters; }
    const LevelFilters& getLevelFilters() { return levelFilters; }

    void applyFilters() {
        levelFilters = displayFilters;
    }

    void setLengthFilter(LengthFilter type) {
        displayFilters.lengthFilter = type;
    }

    void setDifficultyFilter(DifficultyFilter type) {
        displayFilters.diffFilter = type;
    }

    void setCustomLengthFilter(int from, int to) {
        if (from > to) return;
        displayFilters.customLengthFilter = {from, to};
    }

    void setCustomDifficultyFilter(int from, int to) {
        if (from > to) return;
        displayFilters.customDiffFilter = {from, to};
    }

    void setRateFilter(bool rated, bool unrated) {
        if (rated && unrated) return;
        
        displayFilters.rated = rated;
        displayFilters.unrated = unrated;
    }    

    void setCompletedBy(bool completedBy) {
        displayFilters.completedBy = completedBy;
    }

    void setCreatedBy(bool createdBy) {
        displayFilters.createdBy = createdBy;
    }

    void setUserID(int userID) {
        displayFilters.userID = userID;
    }

    void setUsername(const std::string& username) {
        displayFilters.username = username;
    }

    void setCreatorName(const std::string& creatorName) {
        displayFilters.holder = creatorName;
    }

    void clearFilters(bool clearDisplayFilters, bool clearLevelFilters) {
        if (clearDisplayFilters) displayFilters.clear();
        if (clearLevelFilters) levelFilters.clear();
    }
};