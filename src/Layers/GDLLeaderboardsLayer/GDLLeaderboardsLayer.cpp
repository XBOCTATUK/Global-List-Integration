#include "GDLLeaderboardsLayer.hpp"
#include "../../API/Leaderboards/Leaderboards.hpp"
#include "../../Cache/Users/Users.hpp"
#include "../../Events/UserLeaderboardLoadedEvent.hpp"
#include "../../Events/CountryLeaderboardLoadedEvent.hpp"
#include "../../Utils/CalculateCoverScale.hpp"
#include "../../Utils/FlagUtils.hpp"
#include "GDLLeaderboardCell.hpp"
#include <cue/ListBorder.hpp>

using namespace geode::prelude;

GDLLeaderboardsLayer* GDLLeaderboardsLayer::create() {
	auto ret = new GDLLeaderboardsLayer();
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CCScene* GDLLeaderboardsLayer::scene() {
	auto ret = CCScene::create();
	AppDelegate::get()->m_runningScene = ret;
	ret->addChild(GDLLeaderboardsLayer::create());
	return ret;
}

constexpr const char* GLOBAL_LIST_INFO =
"The <cg>most complete</c> and <cf>trusted</c> ranking of the <cr>hardest</c> Geometry Dash demons, maintained by a <cy>dedicated community</c>.";

bool GDLLeaderboardsLayer::init() {
	if (!CCLayer::init()) return false;

	setID("GDLLeaderboardsLayer");

    auto winSize = CCDirector::get()->getWinSize();

	auto gdlBG = CCSprite::create("player-leaderboard-bg.png"_spr);
	gdlBG->setAnchorPoint({ 0.5f, 0.5f });
	gdlBG->setScale(Utils::calculateCoverScale(winSize, gdlBG->getContentSize()));
	gdlBG->setPosition({ winSize.width / 2, winSize.height / 2 });
	gdlBG->setZOrder(0);
	gdlBG->setID("gdl-backgrownd");
	addChild(gdlBG);

    m_listNode = cue::ListNode::create(
        {356.0f, 220.0f},
        cue::Brown,
        cue::ListBorderStyle::SlimLevels
    );
    m_listNode->setPosition(winSize / 2.0f);
	m_listNode->setID("ListNode");
	addChild(m_listNode, 5);

	m_errorMessage = CCLabelBMFont::create("", "bigFont.fnt");
	m_errorMessage->setScale(0.6f);
	m_errorMessage->setPosition(m_listNode->getContentSize() / 2.0f);
	m_errorMessage->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
	m_errorMessage->setID("error-message");
	m_listNode->addChild(m_errorMessage, -1);

	m_searchBar = TailyUI::SearchBar::create(
		"Search players...", TailyUI::SearchBarType::WithCountrySelection,
		[this](auto) { search(); }
	);
	m_searchBar->getDropdownList()->setSelectionCallback(
		[this](size_t index, std::string value) {
			if (m_type == LeaderboardsType::Players) {
				m_lastCountryIndex = index;
			}
			else if (m_type == LeaderboardsType::Countries) {
				m_lastLeaderboardTypeIndex = index;
			}
			else {
				log::warn("Unknown leaderboard type");
			}
		}
	);
	m_searchBar->setPosition({ 0.0f, m_listNode->getContentHeight() - m_searchBar->getContentHeight() });
	m_searchBar->setID("search-bar");
	m_listNode->addChild(m_searchBar, 1);

	auto listNodeScroll = m_listNode->getScrollLayer();
    listNodeScroll->setContentHeight(listNodeScroll->getContentHeight() - m_searchBar->getContentHeight());
	auto listBorder = m_listNode->getChildByType<cue::ListBorder*>(0);
	if (listBorder) listBorder->setZOrder(2);

    auto btnsMenu = CCMenu::create();
	btnsMenu->setPosition({ 0.0f, 0.0f });
	btnsMenu->setID("buttons-menu");
	addChild(btnsMenu, 2);

	m_playersTabBtn = TabButton::create(
		TabBaseColor::Unselected, TabBaseColor::UnselectedDark, "Players",
		this, menu_selector(GDLLeaderboardsLayer::onTabButton)
	);
	m_playersTabBtn->setPosition({ 200.0f, 300.0f });
	m_playersTabBtn->setTag(static_cast<int>(LeaderboardsType::Players));
	m_playersTabBtn->toggle(true);
	btnsMenu->addChild(m_playersTabBtn);

	m_countriesTabBtn = TabButton::create(
		TabBaseColor::Unselected, TabBaseColor::UnselectedDark, "Countries",
		this, menu_selector(GDLLeaderboardsLayer::onTabButton)
	);
	m_countriesTabBtn->setPosition({ winSize.width - 200.0f, 300.0f });
	m_countriesTabBtn->setTag(static_cast<int>(LeaderboardsType::Countries));
	btnsMenu->addChild(m_countriesTabBtn);

	auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	m_backBtn = CCMenuItemExt::createSpriteExtra(
		backSpr, [this](auto) { onBack(); }
	);
	m_backBtn->setPosition({ 24.0f, winSize.height - 23.0f });
	m_backBtn->setID("back-button");
	btnsMenu->addChild(m_backBtn);

	auto leftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	m_leftBtn = CCMenuItemExt::createSpriteExtra(
		leftSpr, [this](auto) { page(m_playersLastPage - 1); }
	);
	m_leftBtn->setPosition({ 24.0f, winSize.height / 2.0f });
	m_leftBtn->setID("prev-page-button");
	btnsMenu->addChild(m_leftBtn);

	auto rightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	rightSpr->setFlipX(true);
	m_rightBtn = CCMenuItemExt::createSpriteExtra(
		rightSpr, [this](auto) { page(m_playersLastPage + 1); }
	);
	m_rightBtn->setPosition({ winSize.width - 24.0f, winSize.height / 2.0f });
	m_rightBtn->setID("next-page-button");
	btnsMenu->addChild(m_rightBtn);

	m_infoBtn = InfoAlertButton::create("Global Demonlist", GLOBAL_LIST_INFO, 1.0f);
	m_infoBtn->setPosition({ 30.0f, 30.0f });
	m_infoBtn->setID("info-button");
	btnsMenu->addChild(m_infoBtn);

	auto refreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	m_refreshBtn = CCMenuItemExt::createSpriteExtra(
		refreshSpr, [this](auto) {
			if (m_type == LeaderboardsType::Players) {
				m_playersLastPage = 1;
				m_playersLastSearchQuery.clear();
				m_lastCountryIndex = SIZE_MAX;

				GDL::API::Leaderboards::getUserLeaderboard();
			}
			else {
				m_countriesLastSearchQuery.clear();
				m_lastLeaderboardTypeIndex = SIZE_MAX;

				GDL::API::Leaderboards::getCountryLeaderboard(CountriesLeaderboardType::Main);
			}

			m_searchBar->getSearchInput()->setString("");
			m_searchBar->getDropdownList()->setSelectedIndex(0);
		}
	);
	m_refreshBtn->setPosition({ winSize.width - refreshSpr->getContentWidth() / 2.0f - 4.0f, refreshSpr->getContentHeight() / 2.0f + 4.0f });
	m_refreshBtn->setID("refresh-button");
	btnsMenu->addChild(m_refreshBtn);

	m_loadingSpinner = LoadingSpinner::create(65.0f);
	m_loadingSpinner->setAnchorPoint({ 0.5f, 0.5f });
	m_loadingSpinner->setPosition(winSize / 2.0f);
	m_loadingSpinner->setID("lodaing-spinner");
	addChild(m_loadingSpinner, 10);

    setKeypadEnabled(true);
	setKeyboardEnabled(true);

	auto userLeaderboardCallback =
	[this](geode::Result<std::vector<int>, APIError> result) {
		if (result.isOk()) {
			log::info("Ok result start");

			populateUserLeaderboard(result.unwrap());
			showLoading(false);

			log::info("Ok result done");
		}
		else {
			log::info("Err result start");

			m_loadingSpinner->setVisible(false);

			auto error = result.err().value();
			auto errorStr = fmt::format("Failed to load demonlist.\nError: {}", error.typeAsString());
			if (error.message != APIMessage::None) {
				errorStr += fmt::format(", Message: {}", error.messageAsString());
			}
			
			m_errorMessage->setString(errorStr.c_str());

			log::info("Err result done");
		}
	};
	
	auto countryLeaderboardCallback =
	[this](geode::Result<std::vector<GDLCountry>, APIError> result) {
		if (result.isOk()) {
			populateCountryLeaderboard(result.unwrap());
			showLoading(false);
		}
		else {
			m_loadingSpinner->setVisible(false);

			auto error = result.err().value();
			auto errorStr = fmt::format("Failed to load demonlist.\nError: {}", error.typeAsString());
			if (error.message != APIMessage::None) {
				errorStr += fmt::format(", Message: {}", error.messageAsString());
			}
			
			m_errorMessage->setString(errorStr.c_str());
		}
	};

	// m_userLeaderboardLoadListener = UserLeaderboardLoadedEvent().listen(
	// 	userLeaderboardCallback
	// );

	// m_countryMainLeaderboardLoadListener = CountryLeaderboardLoadedEvent(CountriesLeaderboardType::Main).listen(
	// 	countryLeaderboardCallback
	// );

	// m_countryAdvancedLeaderboardLoadListener = CountryLeaderboardLoadedEvent(CountriesLeaderboardType::Advanced).listen(
	// 	countryLeaderboardCallback
	// );

	showLoading();
	GDL::API::Leaderboards::getUserLeaderboard();

    return true;
}

void GDLLeaderboardsLayer::onTabButton(cocos2d::CCObject* sender) {
	m_searchBar->getDropdownList()->setOpen(false);

	if (sender->getTag() == static_cast<int>(LeaderboardsType::Players) && m_type != LeaderboardsType::Players) {
		m_countriesTabBtn->toggle(false);
		m_type = LeaderboardsType::Players;

		m_searchBar->setSearchBarType(TailyUI::SearchBarType::WithCountrySelection);
		m_searchBar->getSearchInput()->setPlaceholder("Search players...");

		auto dropdown = m_searchBar->getDropdownList();
		dropdown->setValues(Utils::getCountryNames());
		if (m_lastCountryIndex != SIZE_MAX) dropdown->setSelectedIndex(m_lastCountryIndex);

		showLoading();
		GDL::API::Leaderboards::getUserLeaderboard(
			m_playersLastPage == 0 ? 1 : m_playersLastPage,
			m_playersLastSearchQuery,
			m_lastCountryIndex == SIZE_MAX || dropdown->getSelectedValue() == "All countries" ? "" : dropdown->getSelectedValue()
		);
	}
	else if (sender->getTag() == static_cast<int>(LeaderboardsType::Countries) && m_type != LeaderboardsType::Countries) {
		m_playersTabBtn->toggle(false);
		m_type = LeaderboardsType::Countries;

		m_searchBar->setSearchBarType(TailyUI::SearchBarType::WithLeaderboardTypeSelection);
		m_searchBar->getSearchInput()->setPlaceholder("Search countries...");

		auto dropdown = m_searchBar->getDropdownList();
		dropdown->setValues({ "Main", "Advanced" });
		if (m_lastLeaderboardTypeIndex != SIZE_MAX) dropdown->setSelectedIndex(m_lastLeaderboardTypeIndex);

		showLoading();
		GDL::API::Leaderboards::getCountryLeaderboard(
			m_lastLeaderboardTypeIndex == SIZE_MAX ?
				CountriesLeaderboardType::Main :
			dropdown->getSelectedValue() == "Main" ?
				CountriesLeaderboardType::Main :
				CountriesLeaderboardType::Advanced
		);
	}
}

void GDLLeaderboardsLayer::populateUserLeaderboard(const std::vector<int>& userIDs) {
	m_listNode->getScrollLayer()->m_contentLayer->removeAllChildrenWithCleanup(true);
	if (userIDs.empty()) {
		if (m_playersLastPage > 1) {
			page(m_playersLastPage - 1);
		}
		return;
	}
	else if (userIDs.size() < m_cellsPerPage) {
		m_playersMaxPage = m_playersLastPage;
		m_rightBtn->setVisible(false);
	}

	for (const int& userID : userIDs) {
		auto user = GDL::Cache::Users::getUser(userID);
		if (!user) continue;

		auto userCell = GDLLeaderboardCell::create(*user);
		m_listNode->addCell(userCell);
	}

	m_listNode->getScrollLayer()->scrollToTop();
}

void GDLLeaderboardsLayer::populateCountryLeaderboard(const std::vector<GDLCountry>& countries) {
	m_listNode->getScrollLayer()->m_contentLayer->removeAllChildrenWithCleanup(true);
	auto searchQuery = m_searchBar->getSearchInput()->getString();

	for (const auto& country : countries) {
		if (
			!searchQuery.empty() &&
			!country.title.contains(searchQuery)
		) continue;

		auto userCell = GDLLeaderboardCell::create(country);
		m_listNode->addCell(userCell);
	}

	m_listNode->getScrollLayer()->scrollToTop();
}

void GDLLeaderboardsLayer::search() {
	if (m_type == LeaderboardsType::Players) {
		auto dropdown = m_searchBar->getDropdownList();
		
		showLoading();
		GDL::API::Leaderboards::getUserLeaderboard(
			m_playersLastPage,
			m_searchBar->getSearchInput()->getString(),
			dropdown ? dropdown->getSelectedValue() == "All countries" ? "" : dropdown->getSelectedValue() : ""
		);
	}
	else if (m_type == LeaderboardsType::Countries) {
		auto dropdown = m_searchBar->getDropdownList();
		
		auto typeValue =
		dropdown
		? dropdown->getSelectedValue() == "Main"
			? CountriesLeaderboardType::Main
			: CountriesLeaderboardType::Advanced
		: CountriesLeaderboardType::Main;

		showLoading();
		GDL::API::Leaderboards::getCountryLeaderboard(typeValue);
	}
}

void GDLLeaderboardsLayer::page(int page) {
	if (m_type == LeaderboardsType::Countries) return;

	m_playersLastPage = page < 1 ? 1 : page > m_playersMaxPage ? m_playersMaxPage : page;
	search();
}

void GDLLeaderboardsLayer::showLoading(bool show) {
	m_errorMessage->setVisible(false);
	m_listNode->getScrollLayer()->setVisible(!show);
	m_searchBar->setVisible(!show);
	m_leftBtn->setVisible(
		m_type == LeaderboardsType::Players ?
			!show && m_playersLastPage > 1 :
			false
	);
	m_rightBtn->setVisible(
		m_type == LeaderboardsType::Players ?
			!show && m_playersLastPage < m_playersMaxPage :
			false
	);
	m_refreshBtn->setVisible(!show);

	m_loadingSpinner->setVisible(show);
}

void GDLLeaderboardsLayer::keyDown(enumKeyCodes key, double d) {
	switch (key) {
		case KEY_Left:
	case CONTROLLER_Left:
		if (m_leftBtn->isVisible() && m_type == LeaderboardsType::Players) page(m_playersLastPage - 1);
		break;
	case KEY_Right:
	case CONTROLLER_Right:
		if (m_rightBtn->isVisible() && m_type == LeaderboardsType::Players) page(m_playersLastPage + 1);
		break;
	case KEY_Enter:
		search();
		break;
	default:
		CCLayer::keyDown(key, d);
		break;
	}
}

void GDLLeaderboardsLayer::keyBackClicked() {
	onBack();
}

void GDLLeaderboardsLayer::onExit() {
	// Better Fuck Priority fix
	if (m_searchBar) m_searchBar->m_searchInput->defocus();

	CCLayer::onExit();
}

void GDLLeaderboardsLayer::onBack() {
	CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
}