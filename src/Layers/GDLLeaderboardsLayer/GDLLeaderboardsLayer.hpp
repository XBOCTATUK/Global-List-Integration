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
	std::string m_playersLastSearchQuery = "";
	std::string m_countriesLastSearchQuery = "";
	size_t m_lastCountryIndex = SIZE_MAX;
	size_t m_lastLeaderboardTypeIndex = SIZE_MAX;

    cue::ListNode* m_listNode;
	geode::TabButton* m_playersTabBtn;
	geode::TabButton* m_countriesTabBtn;
    cocos2d::CCLabelBMFont* m_errorMessage;
	geode::LoadingSpinner* m_loadingSpinner;

	TailyUI::SearchBar* m_searchBar;

    cocos2d::CCLabelBMFont* m_pageLabel;
    
	InfoAlertButton* m_infoBtn;
	CCMenuItemSpriteExtra* m_backBtn;
	CCMenuItemSpriteExtra* m_leftBtn;
	CCMenuItemSpriteExtra* m_rightBtn;
	CCMenuItemSpriteExtra* m_refreshBtn;

	int m_cellsPerPage = Settings::increaseLevelsPerPage() ? 50 : 25;

    bool init() override;
	void onTabButton(cocos2d::CCObject* sender);
	void populateUserLeaderboard(const std::vector<int>& userIDs);
	void populateCountryLeaderboard(const std::vector<GDLCountry>& countries);
	void search();
	void page(int page);
	void showLoading(bool show = true);
};