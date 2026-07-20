#pragma once

#include <string>
#include <array>

enum class LengthFilter {
    None, Short, Medium, Long, XL, Custom
};

enum class DifficultyFilter {
    None, Top75, Top150, Top300, Unbounded, Custom
};

struct LevelFilters {
	LengthFilter lengthFilter;
	DifficultyFilter diffFilter;
	std::array<int, 2> customLengthFilter{};
	std::array<int, 2> customDiffFilter{};
	bool rated = false;
	bool unrated = false;
	bool completedBy = false;
	bool createdBy = false;
	std::string username;
	int userID;
	std::string holder;

	bool operator==(LevelFilters const&) const = default;

    bool isDefault() const {
        return *this == LevelFilters{};
    }

	bool isDataRequired() const {
		return rated || unrated || createdBy;
	}

    void clear() {
		*this = {};
	}
};

namespace GDL::Filters {
    const LevelFilters& getDisplayFilters();
    const LevelFilters& getLevelFilters();

    void applyFilters();

    void setLengthFilter(LengthFilter type);
    void setDifficultyFilter(DifficultyFilter type);

    void setCustomLengthFilter(int from, int to);
    void setCustomDifficultyFilter(int from, int to);

    void setRateFilter(bool rated, bool unrated);
    void setCompletedBy(bool completedBy);
    void setCreatedBy(bool createdBy);

    void setUserID(int userID);
    void setUsername(const std::string& username);
    void setCreatorName(const std::string& creatorName);

    void clearFilters(bool clearDisplayFilters = true, bool clearLevelFilters = true);
};