#include "CountryInfoPopup.hpp"
#include "../../UI/nodes.hpp"
#include "../../API/Leaderboards/Leaderboards.hpp"
#include "../../Cache/Leaderboards/Leaderboards.hpp"
#include "../../Events/MainCountryLeaderboardLoadedEvent.hpp"
#include "../../Events/AdvancedCountryLeaderboardLoadedEvent.hpp"
#include "../../Utils/FlagUtils.hpp"

using namespace geode::prelude;

CountryInfoPopup* CountryInfoPopup::create(const GDLCountry& countryData, CountriesLeaderboardType type) {
	auto ret = new CountryInfoPopup();
	if (ret && ret->init(countryData, type)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool CountryInfoPopup::init(const GDLCountry& countryData, CountriesLeaderboardType type) {
    if (!Popup::init(450.0f, 280.0f)) return false;

	m_countryData = countryData;
	auto countryName = string::replace(
		countryData.title,
		" ", "-"
	);

	if (type == CountriesLeaderboardType::Main) {
		m_countryLoadListener = MainCountryLeaderboardLoadedEvent(countryName).listen(
			[this](Result<std::vector<GDLCountryUser>, APIError> result) {
				if (result.isOk()) {
					drawMainTypeUI();
				}
			}
		);

		GDL::API::Leaderboards::getMainCountryLeaderboard(countryName);
	}
	else {
		m_countryLoadListener = AdvancedCountryLeaderboardLoadedEvent(countryName).listen(
			[this](Result<GDLCountryAdvanced, APIError> result) {
				if (result.isOk()) {
					drawAdvancedTypeUI();
				}
			}
		);
		
		GDL::API::Leaderboards::getAdvancedCountryLeaderboard(countryName);
	}

    return true;
}

void CountryInfoPopup::drawBasicInfoUI() {
	auto scrollBG = NineSlice::create("square02b_001.png");
	scrollBG->setColor({ 0, 0, 0 });
	scrollBG->setOpacity(51);
	scrollBG->setContentSize({ m_mainLayer->getContentSize() - ccp(40.0f, 40.0f) });
	m_mainLayer->addChildAtPosition(
		scrollBG,
		Anchor::Center,
		{}
	);

	m_scrollLayer = ScrollLayer::create(scrollBG->getContentSize());
	m_scrollLayer->m_contentLayer->setLayout(
		SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Center)
		->setMainAxisScaling(AxisScaling::Fit)
        ->setGap(5.0f)
        ->ignoreInvisibleChildren(false)
		->setPadding(Padding::uniform(10.0f))
	);
	m_scrollLayer->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChildAtPosition(
		m_scrollLayer,
		Anchor::Center,
		{}
	);

	auto scrollBorder = ListBorders::create();
	scrollBorder->setContentSize(scrollBG->getContentSize());
	m_mainLayer->addChildAtPosition(
		scrollBorder,
		Anchor::Center,
		{}
	);

	auto scrollbar = Scrollbar::create(m_scrollLayer);
	scrollbar->setPosition({
		m_scrollLayer->getPositionX() + m_scrollLayer->getContentWidth() / 2.0f + 6.0f,
		m_scrollLayer->getPositionY()
	});
	m_mainLayer->addChild(scrollbar);

	auto content = m_scrollLayer->m_contentLayer;

	auto basicInfoNode = CCNode::create();
	basicInfoNode->setContentSize({
		m_mainLayer->getContentWidth() - 60.0f, 30.0f
	});
	content->addChild(basicInfoNode);

	auto basicInfoBG = NineSlice::create("square02b_001.png");
	basicInfoBG->setColor({ 0, 0, 0 });
	basicInfoBG->setOpacity(51);
	basicInfoBG->setContentSize(basicInfoNode->getContentSize() * 2.0f);
	basicInfoBG->setScale(0.5f);
	basicInfoNode->addChildAtPosition(
		basicInfoBG,
		Anchor::Center,
		{}
	);

	auto countryName = string::replace(
		m_countryData.title,
		"-", " "
	);

	auto countryNameLabel = CCLabelBMFont::create(
		countryName.c_str(), "bigFont.fnt"
	);
	countryNameLabel->setScale(0.65f);
	countryNameLabel->setAnchorPoint({ 0.0f, 0.5f });
	basicInfoNode->addChildAtPosition(
		countryNameLabel,
		Anchor::Left,
		{ 10.0f, 0.0f }
	);

	auto flagSprName = Utils::getCountrySpriteName(m_countryData.title);

    auto flagSpr = CCSprite::create(flagSprName.c_str());
    flagSpr->setScale(20.0f / flagSpr->getContentHeight());
	flagSpr->setAnchorPoint({ 0.5f, 0.5f });
    basicInfoNode->addChildAtPosition(
		flagSpr,
		Anchor::Right,
		{ -flagSpr->getScaledContentWidth() / 2.0f - 10.0f, 0.0f }
	);
}

void CountryInfoPopup::drawMainTypeUI() {
	drawBasicInfoUI();

	auto mainCountryData = GDL::Cache::Leaderboards::getMainCountryLeaderboard(m_countryData.title);
	if (!mainCountryData) return;

	auto content = m_scrollLayer->m_contentLayer;

	auto playerListNode = TailyUI::createPlayerListNode(
		*mainCountryData,
		m_mainLayer->getContentWidth() - 60.0f
	);
	content->addChild(playerListNode);

	m_scrollLayer->m_contentLayer->updateLayout();
	m_scrollLayer->scrollToTop();
}

void CountryInfoPopup::drawAdvancedTypeUI() {
	drawBasicInfoUI();

	auto advancedCountryData = GDL::Cache::Leaderboards::getAdvancedCountryLeaderboard(m_countryData.title);
	if (!advancedCountryData) return;

	auto content = m_scrollLayer->m_contentLayer;

	auto statsMenu = CCNode::create();
	statsMenu->setLayout(
		RowLayout::create()
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false)
	);
	statsMenu->setContentSize({
		m_mainLayer->getContentWidth() - 60.0f, 40.0f
	});
	statsMenu->setAnchorPoint({ 0.5f, 0.5f });
	content->addChild(statsMenu);

	auto rankNode = TailyUI::createStatNode(
		fmt::format("#{}", m_countryData.placement).c_str(),
		"Rank", "rank-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 10.0f) / 3.0f
	);
	statsMenu->addChild(rankNode);

	auto scoreNode = TailyUI::createStatNode(
		fmt::format("{:.2f}", m_countryData.points).c_str(),
		"Score", "score-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 10.0f) / 3.0f
	);
	statsMenu->addChild(scoreNode);

	auto playersNode = TailyUI::createStatNode(
		fmt::format("{}", advancedCountryData->userCount).c_str(),
		"Players", "green-players-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 10.0f) / 3.0f
	);
	statsMenu->addChild(playersNode);

	statsMenu->updateLayout();

	auto hardestNode = TailyUI::createStatNode(
		fmt::format("#{} {}", advancedCountryData->hardestLevel.placement, advancedCountryData->hardestLevel.name).c_str(),
		"Hardest", "hardest-icon.png"_spr,
		m_mainLayer->getContentWidth() - 60.0f
	);
	content->addChild(hardestNode);

	if (!advancedCountryData->mainList.empty()) {
		auto mainLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->mainList,
			"Main levels",
			"main-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(mainLevelsNode);
	}

	if (!advancedCountryData->extendedList.empty()) {
		auto extendedLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->extendedList,
			"Extended levels",
			"extended-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(extendedLevelsNode);
	}

	if (!advancedCountryData->advancedList.empty()) {
		auto advancedLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->advancedList,
			"Advanced levels",
			"advanced-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(advancedLevelsNode);
	}

	if (!advancedCountryData->unboundedList.empty()) {
		auto unboundedLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->unboundedList,
			"Unbounded levels",
			"unbounded-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(unboundedLevelsNode);
	}

	if (!advancedCountryData->progressList.empty()) {
		auto progressLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->progressList,
			"Progress",
			"progress-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(progressLevelsNode);
	}

	if (!advancedCountryData->verifiedList.empty()) {
		auto verifiedLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->verifiedList,
			"Which are verified",
			"verified-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(verifiedLevelsNode);
	}

	if (!advancedCountryData->uncompletedList.empty()) {
		auto uncompletedLevelsNode = TailyUI::createUserLevelsNode(
			advancedCountryData->uncompletedList,
			"Uncompleted levels",
			"uncompleted-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(uncompletedLevelsNode);
	}

	m_scrollLayer->m_contentLayer->updateLayout();
	m_scrollLayer->scrollToTop();
}