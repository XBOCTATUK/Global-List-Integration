#pragma once

#include "../../API/Levels/Levels.hpp"
#include "../../Filters/Filters.hpp"
#include "../../Utils/CalculateCoverScale.hpp"
#include "../../Utils/IsLevelSuitable.hpp"
#include "../../Settings/Settings.hpp"
#include "../../Popups/FilterPopup/FilterPopup.hpp"
#include "../../Events/PopulateListEvent.hpp"

using namespace geode::prelude;

class GDLListLayer : public CCLayer, SetIDPopupDelegate, LevelManagerDelegate {
public:
	static GDLListLayer* create();
	static CCScene* scene();

	void onBack();
	void keyDown(enumKeyCodes, double delta) override;
	void keyBackClicked() override;
    virtual void onExit() override;

protected:
	ListenerHandle m_demonlistLoadListener;
	ListenerHandle m_populateListListener;
	std::vector<int> m_gdlLevels;

	GJListLayer* m_levelList;
	CCLabelBMFont* m_errorMessage;
	LoadingSpinner* m_loadingSpinner;

	CCNode* m_searchBarMenu;
	TextInput* m_searchBar;

	CCLabelBMFont* m_levelsCountLabel;
	CCLabelBMFont* m_pageLabel;
    
	CCMenu* m_pageMenu;
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

	bool init();
	void populateList();
	void search();
	void page(int page);
	void setupPageInfo(gd::string, const char*) override;
	void showLoading();
	void loadLevelsFinished(CCArray* levels, const char* key, int) override;
	virtual void loadLevelsFailed(char const* key) override;
	void setIDPopupClosed(SetIDPopup*, int) override;
	
	int calculateMaxPage();
};