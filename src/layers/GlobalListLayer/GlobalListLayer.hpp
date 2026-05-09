#pragma once

#include "../../GlobalList/API/Levels.hpp"
#include "../../GlobalList/Filters.hpp"
#include "../../GlobalList/Levels.hpp"

#include "../../popups/FilterPopup/FilterPopup.hpp"

using namespace geode::prelude;

class GlobalListLayer : public cocos2d::CCLayer, SetIDPopupDelegate, LevelManagerDelegate {
public:
	static GlobalListLayer* create();
	static cocos2d::CCScene* scene();

	void keyDown(cocos2d::enumKeyCodes, double d) override;
	void keyBackClicked() override;

	~GlobalListLayer() override;
	friend class FilterPopup;
	friend class LoadingPopup;

protected:
	ListenerHandle m_populateListener;
	GJListLayer* m_levelList;
	cocos2d::CCMenu* m_searchBarMenu;
	geode::TextInput* m_searchBar;
	cocos2d::CCLabelBMFont* m_levelsCountLabel;
	cocos2d::CCLabelBMFont* m_pageLabel;
	CCMenuItemSpriteExtra* m_pageButton;
	CCMenuItemSpriteExtra* m_backButton;
	CCMenuItemSpriteExtra* m_leftButton;
	CCMenuItemSpriteExtra* m_rightButton;
	InfoAlertButton* m_infoButton;
	CCMenuItemSpriteExtra* m_randomButton;
	CCMenuItemSpriteExtra* m_firstButton;
	CCMenuItemSpriteExtra* m_lastButton;
	LoadingCircle* m_loadingCircle;
	size_t m_page = 0;
	size_t m_lvlsPerPage = 10;
	std::string m_query;
	std::vector<int> m_searchResults;

	bool init();
	void populateList(const std::string& query, bool useFilters = true);
	void search();
	void page(size_t page);
	void setupPageInfo(gd::string, const char*) override;
	void showLoading();
	void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int) override;
	virtual void loadLevelsFailed(char const* key) override;
	void setIDPopupClosed(SetIDPopup*, int) override;
};