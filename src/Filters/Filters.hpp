#pragma once

#include <string>
#include <array>

struct LevelFilters {
	std::array<bool, 5> lengthFilter{};
	std::array<bool, 5> diffFilter{};
	std::array<int, 2> customLengthFilter{};
	std::array<int, 2> customDiffFilter{};
	bool rated = false;
	bool unrated = false;
	bool completed = false;
	bool byHolder = false;
	std::string username;
	int userID;
	std::string holder;

	bool operator==(LevelFilters const&) const = default;

    bool isDefault() const {
        return *this == LevelFilters{};
    }

	bool isDataRequired() const {
		return rated || unrated || completed || byHolder;
	}

    void clear() {
		*this = {};
	}
};

enum class LengthFilter {
    Short, Medium, Long, XL, Custom
};

enum class DifficultyFilter {
    Top50, Top150, Top300, Unbounded, Custom
};

namespace GDL::Filters {
    const LevelFilters& getDisplayFilters();
    const LevelFilters& getLevelFilters();

    void applyFilters();

    void setLengthFilter(LengthFilter type, bool select);
    void setDifficultyFilter(DifficultyFilter type, bool select);

    void setCustomLengthFilter(int from, int to);
    void setCustomDifficultyFilter(int from, int to);

    void setRateFilter(bool rated, bool unrated);
    void setCompleted(bool completed);
    void setByHolder(bool byHolder);

    void setUserID(int userID);
    void setUsername(const std::string& username);
    void setHolderName(const std::string& holderName);

    void clearFilters(bool clearDisplayFilters = true, bool clearLevelFilters = true);
};