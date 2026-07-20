#include "GDLListLayer.hpp"
#include <random>

GDLListLayer* GDLListLayer::create() {
	auto ret = new GDLListLayer();
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CCScene* GDLListLayer::scene() {
	auto ret = CCScene::create();
	AppDelegate::get()->m_runningScene = ret;
	ret->addChild(GDLListLayer::create());
	return ret;
}

constexpr const char* GLOBAL_LIST_INFO =
"The <cg>most complete</c> and <cf>trusted</c> ranking of the <cr>hardest</c> Geometry Dash demons, maintained by a <cy>dedicated community</c>.";

//        Do you think I'll remove this comment after two major code refactorings? NOPE, HERE IT IS:
//
//                     This layer contains hiimjasmine00's work (Integrated DemonList).
//                     I'm too silly to do something like this completely on my own =3
//
// I just want to give credit to hiimjasmine00 for the original source code that this project started from.
bool GDLListLayer::init() {
	if (!CCLayer::init()) return false;

	setID("GDLListLayer");

	auto winSize = CCDirector::get()->getWinSize();

	auto gdlBG = CCSprite::create("global-list-bg.png"_spr);
	gdlBG->setAnchorPoint({ 0.5f, 0.5f });
	gdlBG->setScale(Utils::calculateCoverScale(winSize, gdlBG->getContentSize()));
	gdlBG->setPosition({ winSize.width / 2, winSize.height / 2 });
	gdlBG->setZOrder(0);
	gdlBG->setID("gdl-backgrownd");
	addChild(gdlBG);

	m_levelsCountLabel = CCLabelBMFont::create("", "goldFont.fnt");
	m_levelsCountLabel->setAnchorPoint({ 1.0f, 1.0f });
	m_levelsCountLabel->setScale(0.45f);
	m_levelsCountLabel->setPosition({ winSize.width - 7.0f, winSize.height - 3.0f });
	m_levelsCountLabel->setID("level-count-label");
	addChild(m_levelsCountLabel);

	m_levelList = GJListLayer::create(
		nullptr,
		"Global Demonlist",
		{ 0, 0, 0, 180 },
		356.0f, 220.0f, 0);
	m_levelList->setPosition(winSize / 2.0f - m_levelList->getContentSize() / 2.0f);
	m_levelList->setColor({ 194, 114, 62 });
	m_levelList->setID("GJListLayer");
	addChild(m_levelList, 5);

	m_errorMessage = CCLabelBMFont::create("", "bigFont.fnt");
	m_errorMessage->setScale(0.6f);
	m_errorMessage->setPosition(m_levelList->getContentSize() / 2.0f);
	m_errorMessage->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
	m_errorMessage->setID("error-message");
	m_levelList->addChild(m_errorMessage, 5);

	m_searchBarMenu = CCNode::create();
	m_searchBarMenu->setContentSize({ 356.0f, 30.0f });
	m_searchBarMenu->setPosition({ 0.0f, m_levelList->getContentHeight() - m_searchBarMenu->getContentHeight() });
	m_searchBarMenu->setID("search-bar-menu");
	m_levelList->addChild(m_searchBarMenu);

	auto searchBarBG = CCLayerColor::create({ 194, 114, 62, 255 }, 356.0f, 30.0f);
	searchBarBG->setID("search-bar-backgrownd");
	m_searchBarMenu->addChild(searchBarBG);

	auto searchBarContainer = CCMenu::create();
	searchBarContainer->setLayout(
		RowLayout::create()
		->setGap(5.0f)
		->setAutoScale(false)
		->setCrossAxisOverflow(false)
	);
	searchBarContainer->setContentSize(m_searchBarMenu->getContentSize());
	searchBarContainer->setAnchorPoint({ 0.0f, 0.0f });
	searchBarContainer->setPosition({ 0.0f, 0.0f });
	searchBarContainer->setID("search-bar-container");
	m_searchBarMenu->addChild(searchBarContainer);

	m_searchBar = TextInput::create(367.0f, "Search levels...");
	m_searchBar->setMaxCharCount(32);
	m_searchBar->setTextAlign(TextInputAlign::Left);
	m_searchBar->getInputNode()->setLabelPlaceholderScale(0.70f);
	m_searchBar->getInputNode()->setMaxLabelScale(0.70f);
	m_searchBar->setScale(0.75f);
	m_searchBar->setID("search-bar");
	searchBarContainer->addChild(m_searchBar);

	auto searchBtn = CCMenuItemExt::createSpriteExtraWithFrameName("gj_findBtn_001.png", 0.7f, [this](auto) { search(); });
	searchBtn->setID("search-button");
	searchBarContainer->addChild(searchBtn);

	auto filterBtn = CCMenuItemExt::createSpriteExtraWithFilename("GJ_button_01.png", 0.54f, [this](auto) {
		FilterPopup::create()->show();
	});
	auto filterIcon = CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png");
	filterIcon->setPosition(filterBtn->getContentSize() / 2.0f);
	filterIcon->setScale(0.54f);
	filterBtn->addChild(filterIcon);
	filterBtn->setID("filter-button");
	searchBarContainer->addChild(filterBtn);

	searchBarContainer->updateLayout();

	auto btnsMenu = CCMenu::create();
	btnsMenu->setPosition({ 0.0f, 0.0f });
	btnsMenu->setID("buttons-menu");
	addChild(btnsMenu, 2);

	auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	m_backBtn = CCMenuItemExt::createSpriteExtra(backSpr, [this](auto) { onBack(); });
	m_backBtn->setPosition({ 24.0f, winSize.height - 23.0f });
	m_backBtn->setID("back-button");
	btnsMenu->addChild(m_backBtn);

	auto leftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	m_leftBtn = CCMenuItemExt::createSpriteExtra(leftSpr, [this](auto) { page(m_page - 1); });
	m_leftBtn->setPosition({ 24.0f, winSize.height / 2.0f });
	m_leftBtn->setID("prev-page-button");
	btnsMenu->addChild(m_leftBtn);

	auto rightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	rightSpr->setFlipX(true);
	m_rightBtn = CCMenuItemExt::createSpriteExtra(rightSpr, [this](auto) { page(m_page + 1); });
	m_rightBtn->setPosition({ winSize.width - 24.0f, winSize.height / 2.0f });
	m_rightBtn->setID("next-page-button");
	btnsMenu->addChild(m_rightBtn);

	m_infoBtn = InfoAlertButton::create("Global Demonlist", GLOBAL_LIST_INFO, 1.0f);
	m_infoBtn->setPosition({ 30.0f, 30.0f });
	m_infoBtn->setID("info-button");
	btnsMenu->addChild(m_infoBtn);

	auto refreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	auto refreshBtn = CCMenuItemExt::createSpriteExtra(refreshSpr, [this](auto) {
		m_page = 1;
		m_searchBar->setString("");
		GDL::Filters::clearFilters();

		showLoading();
		PopulateListEvent().send();
	});
	refreshBtn->setPosition({ winSize.width - refreshSpr->getContentWidth() / 2.0f - 4.0f, refreshSpr->getContentHeight() / 2.0f + 4.0f });
	refreshBtn->setID("refresh-button");
	btnsMenu->addChild(refreshBtn);

	auto pageSpr = CCSprite::create("GJ_button_02.png");
	pageSpr->setScale(0.7f);
	m_pageLabel = CCLabelBMFont::create("1", "bigFont.fnt");
	m_pageLabel->setScale(0.8f);
	m_pageLabel->setPosition(pageSpr->getContentSize() / 2.0f);
	pageSpr->addChild(m_pageLabel);
	m_pageBtn = CCMenuItemExt::createSpriteExtra(pageSpr, [this](auto) {
		auto popup = SetIDPopup::create(m_page, 1, calculateMaxPage(), "Go to Page", "Go", true, 1, 60.0f, false, false);
		popup->m_delegate = this;
		popup->show();
	});
	m_pageBtn->setID("page-button");

	m_randomBtn = CCMenuItemExt::createSpriteExtraWithFilename("BI_randomBtn_001.png"_spr, 0.9f, [this](auto) {
		static std::mt19937 mt(std::random_device{}());
		page(std::uniform_int_distribution<int>(0, (m_searchResults.size() - 1) / m_lvlsPerPage)(mt));
	});
	m_randomBtn->setID("random-button");
	
	m_pageMenu = CCMenu::createWithItem(m_pageBtn);
	m_pageMenu->setLayout(
		ColumnLayout::create()
		->setAxisAlignment(AxisAlignment::End)
		->setAxisReverse(true)
	);
	m_pageMenu->setAnchorPoint({ 1.0f, 1.0f });
	m_pageMenu->setPosition({ winSize.width - 3.0f, winSize.height - 25.5f });
	m_pageMenu->addChild(m_randomBtn);
	m_pageMenu->setID("page-menu");
	btnsMenu->addChild(m_pageMenu);

	auto lastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	lastArrow->setFlipX(true);
	auto otherLastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	otherLastArrow->setPosition(lastArrow->getContentSize() / 2.0f + CCPoint{ 20.0f, 0.0f });
	otherLastArrow->setFlipX(true);
	lastArrow->addChild(otherLastArrow);
	lastArrow->setScale(0.4f);
	m_lastBtn = CCMenuItemExt::createSpriteExtra(lastArrow, [this](auto) { page(calculateMaxPage()); });
	m_lastBtn->setID("last-button");
	m_pageMenu->addChild(m_lastBtn);

	m_pageMenu->updateLayout();

	auto firstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	auto otherFirstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	otherFirstArrow->setPosition(firstArrow->getContentSize() / 2.0f - ccp( 20.0f, 0.0f ));
	firstArrow->addChild(otherFirstArrow);
	firstArrow->setScale(0.4f);
	m_firstBtn = CCMenuItemExt::createSpriteExtra(firstArrow, [this](auto) { page(1); });
	m_firstBtn->setID("first-button");
	m_firstBtn->setPosition({ 17.5f, m_pageMenu->getPositionY() - m_pageMenu->getContentHeight() + m_lastBtn->getPositionY() });
	btnsMenu->addChild(m_firstBtn);

	m_loadingSpinner = LoadingSpinner::create(65.0f);
	m_loadingSpinner->setAnchorPoint({ 0.5f, 0.5f });
	m_loadingSpinner->setPosition(winSize / 2.0f);
	m_loadingSpinner->setID("lodaing-spinner");
	addChild(m_loadingSpinner, 10);

	setKeypadEnabled(true);
	setKeyboardEnabled(true);
	showLoading();

	m_demonlistLoadListener = DemonlistLoadedEvent().listen(
		[this](Result<const std::vector<int>&, APIError> result) {
			if (result.isOk()) {
				m_gdlLevels = result.unwrap();
				populateList();
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
		}
	);

	m_populateListListener = PopulateListEvent().listen(
		[this]() {
			populateList();
		}
	);

	Settings::setShouldLoadPlacement(true);
	GDL::API::Levels::getDemonlist();

	return true;
}

void GDLListLayer::populateList() {
	GDL::Filters::applyFilters();
	m_searchResults.clear();

	if (m_query.empty() && GDL::Filters::getLevelFilters().isDefault()) {
		for (size_t i = 0; i < m_gdlLevels.size(); i++) {
			int levelID = m_gdlLevels[i];

			m_searchResults.push_back(levelID);
		}
	}
	else {
		for (size_t i = 0; i < m_gdlLevels.size(); i++) {
			auto levelID = m_gdlLevels[i];
			auto level = GDL::Cache::Levels::getLevel(levelID);

			// I hope, I don't forget to rework Utils::isLevelSuitable, it's shit
			if (Utils::isLevelSuitable(level) && level->contains(m_query)) {
				m_searchResults.push_back(level->ingameID);
			}
		}
	}

	if (m_searchResults.empty()) {
		loadLevelsFinished(CCArray::create(), "", 0);
	}
	else {
		auto glm = GameLevelManager::get();
		glm->m_levelManagerDelegate = this;

		size_t begin = m_lvlsPerPage * (m_page-1);
		size_t end = std::min(
			static_cast<size_t>(m_lvlsPerPage * m_page),
			m_searchResults.size()
		);
		
		std::string searchQuery;
		for (size_t i = begin; i < end; i++) {
			if (i > begin) searchQuery += ",";
			searchQuery += std::to_string(m_searchResults[i]);
		}

		auto searchObject = GJSearchObject::create(SearchType::Type26);
		searchObject->m_searchQuery = searchQuery;

		std::string_view key = searchObject->getKey();
		if (auto storedLevels = glm->getStoredOnlineLevels(
			key.substr(std::max<ptrdiff_t>(0, key.size() - 256)).data()
		)) {
			loadLevelsFinished(storedLevels, "", 0);
			setupPageInfo("", "");
		}
		else {
			glm->getOnlineLevels(searchObject);
		}
	}
}

void GDLListLayer::search() {
	auto query = m_searchBar->getString();
	if (m_query != query) {
		showLoading();

		m_page = 1;
		m_query = query;
		populateList();
	}
}

void GDLListLayer::loadLevelsFinished(CCArray* levels, const char*, int) {
	if (auto listView = m_levelList->m_listView) {
		listView->removeFromParent();
		listView->release();
	}

	auto listView = CustomListView::create(levels, BoomListType::Level, 190.0f, 356.0f);
	listView->retain();
	m_levelList->addChild(listView, 6, 9);
	m_levelList->m_listView = listView;

	m_searchBarMenu->setVisible(true);
	m_levelsCountLabel->setVisible(true);
	m_loadingSpinner->setVisible(false);

	if (m_searchResults.size() > m_lvlsPerPage) {
		int maxPage = calculateMaxPage();
		
		m_leftBtn->setVisible(m_page > 1);
		m_rightBtn->setVisible(m_page < maxPage);
		m_firstBtn->setVisible(m_page > 1);
		m_lastBtn->setVisible(m_page < maxPage);
		m_pageMenu->setVisible(true);
	}
	m_pageLabel->limitLabelWidth( 32.0f, 0.8f, 0.4f );
}

void GDLListLayer::loadLevelsFailed(char const* key) {
	FLAlertLayer::create("Loading failed", "Something wrong.", "Ok")->show();
}

void GDLListLayer::page(int page) {
	int maxPage = calculateMaxPage();
	m_page = page <= maxPage ? (page < 1 ? 1 : page) : maxPage;

	showLoading();
	populateList();
}

void GDLListLayer::setupPageInfo(gd::string, const char*) {
	m_levelsCountLabel->setString(
		fmt::format("{} to {} of {}",
			m_lvlsPerPage * (m_page-1) + 1,
			std::min(static_cast<int>(m_searchResults.size()), m_page * m_lvlsPerPage),
			m_searchResults.size()).c_str()
		);
	m_levelsCountLabel->limitLabelWidth( 100.0f, 0.6f, 0.0f );
}

void GDLListLayer::setIDPopupClosed(SetIDPopup*, int page) {
	m_page = std::clamp(page, 1, calculateMaxPage());
	showLoading();
	populateList();
}

void GDLListLayer::showLoading() {
	m_pageLabel->setString(fmt::to_string(m_page).c_str());
	m_loadingSpinner->setVisible(true);

	if (auto listView = m_levelList->m_listView) listView->setVisible(false);
	m_searchBarMenu->setVisible(false);
	m_levelsCountLabel->setVisible(false);
	m_leftBtn->setVisible(false);
	m_rightBtn->setVisible(false);
	m_firstBtn->setVisible(false);
	m_lastBtn->setVisible(false);
	m_pageMenu->setVisible(false);
}

void GDLListLayer::keyDown(enumKeyCodes key, double d) {
	switch (key) {
		case KEY_Left:
	case CONTROLLER_Left:
		if (m_leftBtn->isVisible()) page(m_page - 1);
		break;
	case KEY_Right:
	case CONTROLLER_Right:
		if (m_rightBtn->isVisible()) page(m_page + 1);
		break;
	case KEY_Enter:
		search();
		break;
	default:
		CCLayer::keyDown(key, d);
		break;
	}
}

void GDLListLayer::onBack() {
	Settings::setShouldLoadPlacement(false);
	CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void GDLListLayer::keyBackClicked() {
	onBack();
}

void GDLListLayer::onExit() {
	auto glm = GameLevelManager::get();
	if (glm->m_levelManagerDelegate == this) {
		glm->m_levelManagerDelegate = nullptr;
	}

	// Better Fuck Priority fix
	if (m_searchBar) m_searchBar->defocus();

	CCLayer::onExit();
}

int GDLListLayer::calculateMaxPage() {
	return std::max(1, (static_cast<int>(m_searchResults.size()) + m_lvlsPerPage - 1) / m_lvlsPerPage);
}