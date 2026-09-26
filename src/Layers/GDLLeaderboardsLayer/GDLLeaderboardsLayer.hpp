#pragma once

#include <cstdint>
#include <cue/ListNode.hpp>
#include <string>
#include "../../UI/SearchBar/SearchBar.hpp"
#include "../../Settings/Settings.hpp"
#include "../../Models/GDLCountry.hpp"

enum class LeaderboardsType { Players, Countries };

class GDLLeaderboardsLayer : public cocos2d::CCLayer {
public:
	static GDLLeaderboardsLayer* create();
	static cocos2d::CCScene* scene();

	void keyDown(cocos2d::enumKeyCodes, double delta) override;
	void keyBackClicked() override;
    virtual void onExit() override;

	void onBack();

protected:
    geode::ListenerHandle m_userLeaderboardLoadListener;
    geode::ListenerHandle m_countryMainLeaderboardLoadListener;
    geode::ListenerHandle m_countryAdvancedLeaderboardLoadListener;

	LeaderboardsType m_type = LeaderboardsType::Players;
	int m_playersLastPage = 1;
	int m_playersMaxPage = INT_MAX;
	std::string m_playersLastSearchQuery;
	std::string m_countriesLastSearchQuery;
	size_t m_lastCountryIndex = SIZE_MAX;
	size_t m_lastLeaderboardTypeIndex = SIZE_MAX;

	cocos2d::CCSprite* m_bg = nullptr;
    cue::ListNode* m_listNode = nullptr;
	geode::TabButton* m_playersTabBtn = nullptr;
	geode::TabButton* m_countriesTabBtn = nullptr;
    cocos2d::CCLabelBMFont* m_errorMessage = nullptr;
	geode::LoadingSpinner* m_loadingSpinner = nullptr;

	TailyUI::SearchBar* m_searchBar = nullptr;

    cocos2d::CCLabelBMFont* m_pageLabel = nullptr;
    
	InfoAlertButton* m_infoBtn = nullptr;
	CCMenuItemSpriteExtra* m_backBtn = nullptr;
	CCMenuItemSpriteExtra* m_leftBtn = nullptr;
	CCMenuItemSpriteExtra* m_rightBtn = nullptr;
	CCMenuItemSpriteExtra* m_refreshBtn = nullptr;

	int m_cellsPerPage = Settings::increaseLevelsPerPage() ? 50 : 25;

    bool init() override;
	void onTabButton(cocos2d::CCObject* sender);
	void populateUserLeaderboard(const std::vector<int>& userIDs);
	void populateCountryLeaderboard(const std::vector<GDLCountry>& countries);
	void search();
	void page(int page);
	void showLoading(bool show = true);
};