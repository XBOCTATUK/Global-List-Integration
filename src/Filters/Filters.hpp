#pragma once

enum class LengthFilter {
    None, Short, Medium, Long, XL, Custom
};

enum class DifficultyFilter {
    None, Top75, Top150, Top300, Unbounded, Custom
};

struct DemonlistFilters {
    LengthFilter length = LengthFilter::None;
    DifficultyFilter difficulty = DifficultyFilter::None;

    std::array<int, 2> customLength = { 1, INT_MAX };
    std::array<int, 2> customDifficulty = { 1, INT_MAX };

    bool rated = false;
    bool unrated = false;
    bool completedBy = false;
    bool createdBy = false;

    std::string username;
    int userID = 0;
    std::string creatorName;

    bool operator==(const DemonlistFilters& other) const = default;
};

struct LeaderboardFilters {
    std::string playerSearchQuery;
    size_t countrySelectedIndex = 0;
    std::string countrySearchQuery;
    size_t leaderboardTypeIndex = 0;

    bool operator==(const LeaderboardFilters& other) const = default;
};

namespace GDL {
    class Filters {
    public:
        static LengthFilter getLength(bool applied);
        static DifficultyFilter getDifficulty(bool applied);

        static int getCustomMinLength(bool applied);
        static int getCustomMaxLength(bool applied);
        static int getCustomMinDifficulty(bool applied);
        static int getCustomMaxDifficulty(bool applied);

        static bool getRated(bool applied);
        static bool getUnrated(bool applied);
        static bool getCompletedBy(bool applied);
        static bool getCreatedBy(bool applied);

        static const std::string& getUsername(bool applied);
        static int getUserID(bool applied);
        static const std::string& getCreatorName(bool applied);

        static void setLength(LengthFilter value);
        static void setDifficulty(DifficultyFilter value);

        static void setCustomLength(int min, int max);
        static void setCustomDifficulty(int min, int max);

        static void setRated(bool value);
        static void setUnrated(bool value);
        static void setCompletedBy(bool value);
        static void setCreatedBy(bool value);

        static void setUsername(std::string value);
        static void setUserID(int value);
        static void setCreatorName(std::string value);

        static bool isDemonlistFiltersDefault();
        static bool isLevelDataRequired();
        static void applyDemonlistFilters();
        static void clearDemonlistFilters();

        static const std::string& getPlayerSearchQuery(bool applied);
        static size_t getCountrySelectedIndex(bool applied);
        static const std::string& getCountrySearchQuery(bool applied);
        static size_t getLeaderboardTypeIndex(bool applied);

        static void setPlayerSearchQuery(std::string value);
        static void setCountrySelectedIndex(size_t value);
        static void setCountrySearchQuery(std::string value);
        static void setLeaderboardTypeIndex(size_t value);

        static void applyLeaderboardFilters();
        static void clearLeaderboardFilters();

    private:
        static const DemonlistFilters& demonlist(bool applied);
        static const LeaderboardFilters& leaderboard(bool applied);

        static DemonlistFilters m_savedDemonlist;
        static DemonlistFilters m_appliedDemonlist;

        static LeaderboardFilters m_savedLeaderboard;
        static LeaderboardFilters m_appliedLeaderboard;
    };
}