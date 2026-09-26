#pragma once

enum class LoadingStage {
    UserInfo, Levels
};

class LoadingPopup : public geode::Popup, LevelManagerDelegate {
public:
	static LoadingPopup* create();

protected:
	geode::ListenerHandle m_userLeaderboardLoadListener;
	geode::ListenerHandle m_userLoadListener;

	std::vector<int> m_requiredLevels;

	bool m_loading = false;
	bool m_userInfoLoading = false;

	size_t m_totalSteps = 0;
	size_t m_totalBatches = 0;
	size_t m_completedSteps = 0;

	std::vector<LoadingStage> m_stages;
	size_t m_currentStage = 0;
	size_t m_currentBatch = 0;

	Slider* m_loadingProgress = nullptr;
	cocos2d::CCLabelBMFont* m_loadingLabel = nullptr;

	bool init() override;
	void startLoading();
	void startNextStage();
	void finishStage();
	void startUserLoading();
	void startLevelsLoading();
	void loadBatch(float);
	void afterLoading();
	void updateProgress();

	virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const* key) override;
	virtual void loadLevelsFailed(char const* key) override;
	virtual void keyBackClicked() override;
	virtual void onClose(cocos2d::CCObject*) override;
};