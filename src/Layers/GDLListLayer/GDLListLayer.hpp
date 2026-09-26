#pragma once

#include "../../Settings/Settings.hpp"
#include "../../UI/SearchBar/SearchBar.hpp"

class GDLListLayer : public cocos2d::CCLayer, SetIDPopupDelegate, LevelManagerDelegate {
public:
	static GDLListLayer* create();
	static cocos2d::CCScene* scene();

	void keyDown(cocos2d::enumKeyCodes, double delta) override;
	void keyBackClicked() override;
    virtual void onExit() override;
	
	void onBack();

protected:
	geode::ListenerHandle m_demonlistLoadListener;
	geode::ListenerHandle m_populateListListener;
	std::vector<int> m_gdlLevels;

	GJListLayer* m_levelList = nullptr;
	cocos2d::CCLabelBMFont* m_errorMessage = nullptr;
	geode::LoadingSpinner* m_loadingSpinner = nullptr;

	TailyUI::SearchBar* m_searchBar = nullptr;

	cocos2d::CCLabelBMFont* m_levelsCountLabel = nullptr;
	cocos2d::CCLabelBMFont* m_pageLabel = nullptr;
    
	cocos2d::CCMenu* m_pageMenu = nullptr;
	InfoAlertButton* m_infoBtn = nullptr;
	CCMenuItemSpriteExtra* m_backBtn = nullptr;
	CCMenuItemSpriteExtra* m_pageBtn = nullptr;
	CCMenuItemSpriteExtra* m_leftBtn = nullptr;
	CCMenuItemSpriteExtra* m_rightBtn = nullptr;
	CCMenuItemSpriteExtra* m_randomBtn = nullptr;
	CCMenuItemSpriteExtra* m_firstBtn = nullptr;
	CCMenuItemSpriteExtra* m_lastBtn = nullptr;
    
	int m_page = 1;
	int m_lvlsPerPage = Settings::increaseLevelsPerPage() ? 25 : 10;
	std::string m_query;
	std::vector<int> m_searchResults;

	bool init() override;
	void populateList();
	std::vector<int> getSuitableLevels();
	void search();
	void page(int page);
	void setupPageInfo(gd::string, const char*) override;
	void showLoading(bool show = true);
	void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int) override;
	virtual void loadLevelsFailed(char const* key) override;
	void setIDPopupClosed(SetIDPopup*, int) override;
	
	int calculateMaxPage();
};