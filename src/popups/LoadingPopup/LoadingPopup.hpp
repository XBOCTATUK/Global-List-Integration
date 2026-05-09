#pragma once

#include <Geode/Geode.hpp>

#include "../../events/PopulateListEvent.hpp"
#include "../../events/UserCachedEvent.hpp"
#include "../../events/CloseEvent.hpp"

#include "../../GlobalList/Filters.hpp"
#include "../../GlobalList/Levels.hpp"
#include "../../GlobalList/API/Users.hpp"
#include "../../GlobalList/Cache/LevelCache.hpp"

using namespace geode::prelude;

class LoadingPopup : public geode::Popup, LevelManagerDelegate {
public:
	static LoadingPopup* create();

protected:
	ListenerHandle m_userEvent;
	std::vector<int> m_requiredLevels;
	bool m_loading = false;
	bool m_playerInfoLoading = false;
	int m_currentBatch = 0;

	Slider* m_loadingProgress;
	CCLabelBMFont* m_loadingLabel;

	bool init();
	void getLevels();
	void loadBatch(float);
	void afterLoading();
	virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const* key) override;
	virtual void loadLevelsFailed(char const* key) override;
	virtual void keyBackClicked() override;
	virtual void onClose(cocos2d::CCObject*) override;
};