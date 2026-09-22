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

	GJListLayer* m_levelList;
	cocos2d::CCLabelBMFont* m_errorMessage;
	geode::LoadingSpinner* m_loadingSpinner;

	TailyUI::SearchBar* m_searchBar;

	cocos2d::CCLabelBMFont* m_levelsCountLabel;
	cocos2d::CCLabelBMFont* m_pageLabel;
    
	cocos2d::CCMenu* m_pageMenu;
	InfoAlertButton* m_infoBtn;
	CCMenuItemSpriteExtra* m_backBtn;
	CCMenuItemSpriteExtra* m_pageBtn;
	CCMenuItemSpriteExtra* m_leftBtn;
	CCMenuItemSpriteExtra* m_rightBtn;
	CCMenuItemSpriteExtra* m_randomBtn;
	CCMenuItemSpriteExtra* m_firstBtn;
	CCMenuItemSpriteExtra* m_lastBtn;
    
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
	void showLoading();
	void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int) override;
	virtual void loadLevelsFailed(char const* key) override;
	void setIDPopupClosed(SetIDPopup*, int) override;
	
	int calculateMaxPage();
};