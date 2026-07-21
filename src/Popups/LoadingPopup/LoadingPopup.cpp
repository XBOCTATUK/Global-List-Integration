#include "LoadingPopup.hpp"
#include "../../Filters/Filters.hpp"
#include "../../Cache/Levels/Levels.hpp"
#include "../../Cache/Users/Users.hpp"
#include "../../Cache/GameLevels/GameLevels.hpp"
#include "../../API/Leaderboards/Leaderboards.hpp"
#include "../../API/Users/Users.hpp"
#include "../../Events/CloseFiltersEvent.hpp"
#include "../../Events/PopulateListEvent.hpp"
#include "../../Events/UserLoadedEvent.hpp"
#include "../../Events/UserLeaderboardLoadedEvent.hpp"
#include "../../Utils/Failure.hpp"

LoadingPopup* LoadingPopup::create() {
	auto ret = new LoadingPopup();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool LoadingPopup::init() {
	if (!Popup::init(200.0f, 120.0f)) return false;

	setID("loading-menu");
	setTitle("Level data loading");

	m_closeBtn->setVisible(false);

	m_loadingProgress = Slider::create(nullptr, nullptr);
	m_loadingProgress->m_touchLogic->m_thumb->setVisible(false);
	m_loadingProgress->ignoreAnchorPointForPosition(false);
	m_loadingProgress->setValue(0.0f);
	m_loadingProgress->setAnchorPoint({ 0.5f, 0.5f });
	m_loadingProgress->setPosition({ 100.0f, 58.0f });
	m_loadingProgress->setContentSize({ 0.0f, 0.0f });
	m_loadingProgress->setScale(0.8f);
	m_loadingProgress->setID("loading-progress");
	m_mainLayer->addChild(m_loadingProgress);

	m_loadingLabel = CCLabelBMFont::create("Start loading...", "bigFont.fnt");
	m_loadingLabel->setPosition({ 100.0f, 70.0f });
	m_loadingLabel->setScale(0.4f);
	m_loadingLabel->setID("loading-text");
	m_mainLayer->addChild(m_loadingLabel);

	auto cancelSpr = ButtonSprite::create("Cancel", 0.8f);
	auto cancelBtn = CCMenuItemExt::createSpriteExtra(cancelSpr, [this](auto) {
		m_userLeaderboardLoadedListener.destroy();
		m_userLoadedListener.destroy();

		m_loading = false;
		onClose(nullptr);
	});
	cancelBtn->setPosition({ 100.0f, 24.0f });
	cancelBtn->m_baseScale = cancelBtn->getScale();
	cancelBtn->setID("cancel-button");
	m_buttonMenu->addChild(cancelBtn);
	
	auto& displayFilters = GDL::Filters::getDisplayFilters();

	m_userLeaderboardLoadedListener = UserLeaderboardLoadedEvent().listen(
		[this, &displayFilters](Result<const std::vector<int>*, APIError> result) {
			if (result.isOk()) {
				auto users = result.unwrap();

				auto username = string::toLower(displayFilters.username);
				for (const auto& userID : *users) {
					auto user = GDL::Cache::Users::getUser(userID);

					if (string::toLower(user->username) == username) {
						GDL::Filters::setUserID(user->id);

						m_userLoadedListener.destroy();
						m_userLoadedListener = UserLoadedEvent(user->id).listen(
							[this](Result<const GDLUser*, APIError> result) {
								if (result.isOk()) {
									m_completedSteps++;
									updateProgress();
									finishStage();
								}
								else {
									auto error = result.err().value();
									Utils::failure(
										"User records load failed",
										fmt::format(
											"Failed to load user records data.\nError: {}.\nMessage: {}.\nTry again later.",
											error.typeAsString(),
											error.messageAsString()
										).c_str()
									);

									m_loading = false;
									onClose(nullptr);
								}
							}
						);

						GDL::API::Users::getUser(user->id);

						return;
					}
				}
			}
			else {
				auto error = result.err().value();
				Utils::failure(
					"User load failed",
					fmt::format(
						"Failed to load user data.\nError: {}.\nMessage: {}.\nTry again later.",
						error.typeAsString(),
						error.messageAsString()
					).c_str()
				);

				m_loading = false;
				onClose(nullptr);
			}
		}
	);

	startLoading();

	return true;
}

void LoadingPopup::startLoading() {
	m_loading = true;

	m_totalSteps = 0;
	m_totalBatches = 0;
	m_completedSteps = 0;
	m_currentStage = 0;
	m_currentBatch = 0;
	m_requiredLevels.clear();

	for (auto levelID : GDL::Cache::Levels::getDemonlist()) {
		m_requiredLevels.push_back(levelID);
	}

	auto& displayFilters = GDL::Filters::getDisplayFilters();
	if (displayFilters.completedBy && !displayFilters.username.empty()) {
		m_stages.push_back(LoadingStage::UserInfo);
		m_totalSteps++;
	}
	
	if (displayFilters.isDataRequired() && !m_requiredLevels.empty()) {
		GDL::Cache::GameLevels::clear();

		m_stages.push_back(LoadingStage::Levels);
		m_totalBatches = (m_requiredLevels.size() + 99) / 100;
    	m_totalSteps += m_totalBatches;
	}
	
	startNextStage();
}

void LoadingPopup::startNextStage() {
	if (m_currentStage >= m_stages.size()) {
        afterLoading();
        return;
    }

	if (m_stages[m_currentStage] == LoadingStage::UserInfo) {
		startUserLoading();
	}
	else if (m_stages[m_currentStage] == LoadingStage::Levels) {
		startLevelsLoading();
	}
}

void LoadingPopup::finishStage() {
    m_currentStage++;
    startNextStage();
}

void LoadingPopup::startUserLoading() {
	updateProgress();
	
    auto& displayFilters = GDL::Filters::getDisplayFilters();
    GDL::API::Leaderboards::getUserLeaderboard(1, displayFilters.username);
}

void LoadingPopup::startLevelsLoading() {
    m_currentBatch = 0;
    loadBatch(0.0f);
}

void LoadingPopup::loadBatch(float) {
	if (!m_loading) return;

	updateProgress();

	size_t begin = m_currentBatch * 100;
	size_t end = std::min(begin + 100, m_requiredLevels.size());

	std::string searchQuery;
	for (size_t i = begin; i < end; i++) {
		if (i > begin) searchQuery += ",";
		searchQuery += std::to_string(m_requiredLevels[i]);
	}

	auto searchObj = GJSearchObject::create(SearchType::Type26, searchQuery);

	auto glm = GameLevelManager::get();
	glm->m_levelManagerDelegate = this;
	glm->getOnlineLevels(searchObj);
}

void LoadingPopup::afterLoading() {
	if (std::find(m_stages.begin(), m_stages.end(), LoadingStage::Levels) != m_stages.end()) {
		GDL::Cache::GameLevels::updateCachedAt();
	}

	CloseFiltersEvent().send();
	PopulateListEvent().send();

	Notification::create("Level data loaded successfully.", NotificationIcon::Success, 2.0f)->show();

	m_loading = false;
	onClose(nullptr);
}

void LoadingPopup::updateProgress() {
    if (m_totalSteps == 0) return;

	auto& displayFilters = GDL::Filters::getDisplayFilters();
	
	if (m_stages[m_currentStage] == LoadingStage::UserInfo) {
		m_loadingLabel->setString(
			fmt::format("Loading user '{}'", displayFilters.username).c_str()
		);
	}
	else if (m_stages[m_currentStage] == LoadingStage::Levels) {
		m_loadingLabel->setString(
			fmt::format("Loading {}/{} batch", m_currentBatch + 1, m_totalBatches).c_str()
		);
	}

    m_loadingProgress->setValue(
        static_cast<float>(m_completedSteps) /
        static_cast<float>(m_totalSteps)
    );
}

void LoadingPopup::loadLevelsFinished(CCArray* levels, char const* key) {
	for (size_t i = 0; i < levels->count(); i++) {
		auto levelData = static_cast<GJGameLevel*>(levels->objectAtIndex(i));
		
		GameLevel gameLevel = {
			levelData->m_levelName, levelData->m_creatorName,
			levelData->m_levelID, levelData->m_stars.value() > 0
		};

		GDL::Cache::GameLevels::setGameLevel(std::move(gameLevel));
	}

	m_currentBatch++;
	m_completedSteps++;

	if (m_currentBatch == m_totalBatches) {
		finishStage();
		return;
	}

	scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&LoadingPopup::loadBatch), 1.0f);
}

void LoadingPopup::loadLevelsFailed(char const* key) {
	Utils::failure("Loading failed", "Something wrong. Try again later.");

	m_loading = false;
	onClose(nullptr);
}

void LoadingPopup::keyBackClicked() { onClose(nullptr); }

void LoadingPopup::onClose(cocos2d::CCObject*) {
	if (!m_loading) {
		auto glm = GameLevelManager::get();
		if (glm->m_levelManagerDelegate == this) {
			glm->m_levelManagerDelegate = nullptr;
		}

		Popup::onClose(nullptr);
	}
}