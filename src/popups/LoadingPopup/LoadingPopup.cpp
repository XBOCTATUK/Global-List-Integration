#include "./LoadingPopup.hpp"

LoadingPopup* LoadingPopup::create() {
	auto ret = new LoadingPopup();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool LoadingPopup::init() {
	if (!Popup::init(200.0f, 120.0f)) return false;

	this->setID("loading-menu");
	this->setZOrder(102);
	this->setTitle("Level data loading");

	m_closeBtn->setVisible(false);

	m_loadingProgress = Slider::create(nullptr, nullptr);
	m_loadingProgress->m_touchLogic->m_thumb->setVisible(false);
	m_loadingProgress->ignoreAnchorPointForPosition(false);
	m_loadingProgress->setValue(0.0f);
	m_loadingProgress->setAnchorPoint({ 0.5f, 0.5f });
	m_loadingProgress->setPosition({ 100.0f, 58.0f });
	m_loadingProgress->setContentSize({ 0.0f, 0.0f });
	m_loadingProgress->setScale(0.8f);

	m_loadingLabel = CCLabelBMFont::create("Start loading...", "bigFont.fnt");
	m_loadingLabel->setPosition({ 100.0f, 70.0f });
	m_loadingLabel->setScale(0.4f);

	auto cancelSpr = ButtonSprite::create("Cancel");
	auto cancelBtn = CCMenuItemExt::createSpriteExtra(cancelSpr, [this](auto) {
		m_loading = false;
		auto glm = GameLevelManager::get();
		glm->m_levelManagerDelegate = nullptr;
		onClose(this);
	});
	cancelBtn->setScale(0.8f);
	cancelBtn->setPosition({ 100.0f, 24.0f });
	cancelBtn->m_baseScale = cancelBtn->getScale();

	m_mainLayer->addChild(m_loadingProgress);
	m_mainLayer->addChild(m_loadingLabel);
	m_buttonMenu->addChild(cancelBtn);
	
	getLevels();

	auto& displayFilters = GlobalList::Filters::getDisplayFilters();
	m_userEvent = UserCachedEvent(displayFilters.username).listen(
		[this]() {
			if (m_requiredLevels.size() != 0) {
				m_loading = true;
				m_currentBatch = 0;
				loadBatch(0.0f);
			}
			else afterLoading();
		}
	);

	return true;
}

void LoadingPopup::getLevels() {
	auto glm = GameLevelManager::sharedState();
	GlobalList::Cache::levelDataClear();
	m_requiredLevels.clear();

	for (size_t i = 0; i < GlobalList::Levels::size(); i++) {
			int levelID = GlobalList::Levels::getLevelByIndex(i)->levelID;

			m_requiredLevels.push_back(levelID);
		}

	auto& displayFilters = GlobalList::Filters::getDisplayFilters();
	if (displayFilters.completed && !displayFilters.username.empty())
		GlobalList::API::getUser(displayFilters.username, true);
	else if (m_requiredLevels.size() != 0) {
		m_loading = true;
		m_currentBatch = 0;
		loadBatch(0.0f);
	}
	else afterLoading();
}

void LoadingPopup::loadBatch(float) {
	if (!m_loading) return;

	size_t begin = m_currentBatch * 100;
	size_t end = std::min(begin + 100, m_requiredLevels.size());

	if (begin > m_requiredLevels.size()) {
		m_loading = false;
		afterLoading();
		Notification::create("Level data loaded successfully.", NotificationIcon::Success, 2.0f)->show();

		auto time = std::chrono::high_resolution_clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>( time.time_since_epoch() );
		int64_t timestamp = seconds.count();
		Mod::get()->setSavedValue<int64_t>("lastLevelDataUpdate", timestamp);

		return;
	}

	std::string levelString;
	for (size_t i = begin; i < end; i++) {
		levelString += std::to_string(m_requiredLevels[i]);
		if (i < end - 1) levelString += ",";
	}

	m_loadingProgress->setValue( (float) (m_currentBatch + (m_playerInfoLoading ? 1 : 0)) / (m_requiredLevels.size() / 100 + 1 + (m_playerInfoLoading ? 1 : 0)) );
	m_loadingProgress->updateLayout();
	m_loadingLabel->setString(fmt::format("Loading {}/{} batch", m_currentBatch + 1, m_requiredLevels.size() / 100 + 1).c_str());

	auto searchObj = GJSearchObject::create(SearchType::Type26, levelString);

	auto glm = GameLevelManager::get();
	glm->m_levelManagerDelegate = this;
	glm->getOnlineLevels(searchObj);
}

void LoadingPopup::afterLoading() {
	GlobalList::Filters::applyFilters();

	PopulateListEvent().send();
	MyCloseEvent(PopupType::FilterPopup).send();
	GlobalList::Cache::updateLevelLoadTime();

	onClose(this);
}

void LoadingPopup::loadLevelsFinished(CCArray* levels, char const* key) {
	bool isAllLoaded = true;
	for (int i = 0; i < levels->count(); i++) {
		if (auto levelData = typeinfo_cast<GJGameLevel*>(levels->objectAtIndex(i))) {
			LevelData data = {
				levelData->m_stars > 0,
				levelData->m_stars == 0,
				levelData->m_creatorName
			};

			GlobalList::Cache::saveLevelData(levelData->m_levelID, data);
		}
	}

	m_currentBatch++;
	this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&LoadingPopup::loadBatch), 1.0f);
}

void LoadingPopup::loadLevelsFailed(char const* key) {
	m_loading = false;
	FLAlertLayer::create("Loading failed", "Something wrong. Try again later.", "Ok")->show();
}

void LoadingPopup::keyBackClicked() { onClose(this); }

void LoadingPopup::onClose(cocos2d::CCObject*) {
	if (!m_loading) Popup::onClose(this);
}