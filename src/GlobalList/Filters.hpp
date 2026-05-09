#pragma once

#include <string>
#include <array>

struct LevelFilters {
	std::array<bool, 5> lengthFilter = { false, false, false, false, false }; // Short, Medium, Long, XL, Custom
	std::array<bool, 5> diffFilter = { false, false, false, false, false }; // Top 50, Top 150, Top 300, Beyond, Custom
	std::array<size_t, 2> customLengthFilter = { 0, 0 }; // begin, end
	std::array<size_t, 2> customDiffFilter = { 0, 0 }; // begin, end
	bool rated = false;
	bool unrated = false;
	bool completed = false;
	bool byHolder = false;
	std::string username = "";
	std::string holder = "";

	bool isDefault() const {
		return
			!this->lengthFilter[0] && !this->lengthFilter[1] && !this->lengthFilter[2] && !this->lengthFilter[3] && !this->lengthFilter[4] &&
			!this->diffFilter[0] && !this->diffFilter[1] && !this->diffFilter[2] && !this->diffFilter[3] && !this->diffFilter[4] &&
			!this->rated && !this->unrated && !this->completed && !this->byHolder;
	}

	bool isDataRequired() const {
		return this->rated || this->unrated || this->completed || this->byHolder;
	}

    void clear() {
        *this = LevelFilters{};
    }
};

enum class LengthFilter {
    Short, Medium, Long, XL, Custom
};

enum class DifficultyFilter {
    Top50, Top150, Top300, Unbounded, Custom
};

namespace GlobalList::Filters {
    const LevelFilters& getDisplayFilters();
    const LevelFilters& getLevelFilters();

    void applyFilters();

    void setLengthFilter(LengthFilter type, bool select);
    void setDifficultyFilter(DifficultyFilter type, bool select);

    void setCustomLengthFilter(size_t from, size_t to);
    void setCustomDifficultyFilter(size_t from, size_t to);

    void setRated(bool rated) ;
    void setUnrated(bool unrated);
    void setCompleted(bool completed);
    void setByHolder(bool byHolder);

    void setUsername(const std::string& username);
    void setHolderName(const std::string& holderName);

    void clearFilters(bool clearDisplayFilters = true, bool clearLevelFilters = true);
};