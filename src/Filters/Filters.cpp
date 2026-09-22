#include "Filters.hpp"

DemonlistFilters GDL::Filters::m_savedDemonlist;
DemonlistFilters GDL::Filters::m_appliedDemonlist;

LeaderboardFilters GDL::Filters::m_savedLeaderboard;
LeaderboardFilters GDL::Filters::m_appliedLeaderboard;


LengthFilter GDL::Filters::getLength(bool applied) {
    return demonlist(applied).length;
}

DifficultyFilter GDL::Filters::getDifficulty(bool applied) {
    return demonlist(applied).difficulty;
}

int GDL::Filters::getCustomMinLength(bool applied) {
    return demonlist(applied).customLength[0];
}

int GDL::Filters::getCustomMaxLength(bool applied) {
    return demonlist(applied).customLength[1];
}

int GDL::Filters::getCustomMinDifficulty(bool applied) {
    return demonlist(applied).customDifficulty[0];
}

int GDL::Filters::getCustomMaxDifficulty(bool applied) {
    return demonlist(applied).customDifficulty[1];
}

bool GDL::Filters::getRated(bool applied) {
    return demonlist(applied).rated;
}

bool GDL::Filters::getUnrated(bool applied) {
    return demonlist(applied).unrated;
}

bool GDL::Filters::getCompletedBy(bool applied) {
    return demonlist(applied).completedBy;
}

bool GDL::Filters::getCreatedBy(bool applied) {
    return demonlist(applied).createdBy;
}

const std::string& GDL::Filters::getUsername(bool applied) {
    return demonlist(applied).username;
}

int GDL::Filters::getUserID(bool applied) {
    return demonlist(applied).userID;
}

const std::string& GDL::Filters::getCreatorName(bool applied) {
    return demonlist(applied).creatorName;
}

const std::string& GDL::Filters::getPlayerSearchQuery(bool applied) {
    return leaderboard(applied).playerSearchQuery;
}

size_t GDL::Filters::getCountrySelectedIndex(bool applied) {
    return leaderboard(applied).countrySelectedIndex;
}

const std::string& GDL::Filters::getCountrySearchQuery(bool applied) {
    return leaderboard(applied).countrySearchQuery;
}

size_t GDL::Filters::getLeaderboardTypeIndex(bool applied) {
    return leaderboard(applied).leaderboardTypeIndex;
}


void GDL::Filters::setLength(LengthFilter value) {
    m_savedDemonlist.length = value;
}

void GDL::Filters::setDifficulty(DifficultyFilter value) {
    m_savedDemonlist.difficulty = value;
}

void GDL::Filters::setCustomLength(int min, int max) {
    m_savedDemonlist.customLength = { min, max };
}

void GDL::Filters::setCustomDifficulty(int min, int max) {
    m_savedDemonlist.customDifficulty = { min, max };
}

void GDL::Filters::setRated(bool value) {
    m_savedDemonlist.rated = value;
}

void GDL::Filters::setUnrated(bool value) {
    m_savedDemonlist.unrated = value;
}

void GDL::Filters::setCompletedBy(bool value) {
    m_savedDemonlist.completedBy = value;
}

void GDL::Filters::setCreatedBy(bool value) {
    m_savedDemonlist.createdBy = value;
}

void GDL::Filters::setUsername(std::string value) {
    m_savedDemonlist.username = std::move(value);
}

void GDL::Filters::setUserID(int value) {
    m_savedDemonlist.userID = value;
}

void GDL::Filters::setCreatorName(std::string value) {
    m_savedDemonlist.creatorName = std::move(value);
}

void GDL::Filters::setPlayerSearchQuery(std::string value) {
    m_savedLeaderboard.playerSearchQuery = std::move(value);
}

void GDL::Filters::setCountrySelectedIndex(size_t value) {
    m_savedLeaderboard.countrySelectedIndex = value;
}

void GDL::Filters::setCountrySearchQuery(std::string value) {
    m_savedLeaderboard.countrySearchQuery = std::move(value);
}

void GDL::Filters::setLeaderboardTypeIndex(size_t value) {
    m_savedLeaderboard.leaderboardTypeIndex = value;
}

bool GDL::Filters::isDemonlistFiltersDefault() {
    return m_appliedDemonlist == DemonlistFilters{};
}

bool GDL::Filters::isLevelDataRequired() {
    return m_savedDemonlist.rated || m_savedDemonlist.unrated || m_savedDemonlist.createdBy;
}

void GDL::Filters::applyDemonlistFilters() {
    m_appliedDemonlist = m_savedDemonlist;
}

void GDL::Filters::clearDemonlistFilters() {
    m_savedDemonlist = {};
    m_appliedDemonlist = {};
}

void GDL::Filters::applyLeaderboardFilters() {
    m_appliedLeaderboard = m_savedLeaderboard;
}

void GDL::Filters::clearLeaderboardFilters() {
    m_savedLeaderboard = {};
    m_appliedLeaderboard = {};
}

const DemonlistFilters& GDL::Filters::demonlist(bool applied) {
    return applied ? m_appliedDemonlist : m_savedDemonlist;
}

const LeaderboardFilters& GDL::Filters::leaderboard(bool applied) {
    return applied ? m_appliedLeaderboard : m_savedLeaderboard;
}