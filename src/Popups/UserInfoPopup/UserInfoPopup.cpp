#include "UserInfoPopup.hpp"
#include "../../UI/nodes.hpp"
#include "../../API/Users/Users.hpp"
#include "../../Cache/Users/Users.hpp"
#include "../../Events/UserLoadedEvent.hpp"
#include "../../Utils/FlagUtils.hpp"
#include "../../Utils/BadgeUtils.hpp"
#include "../../Utils/Alert.hpp"

using namespace geode::prelude;

UserInfoPopup* UserInfoPopup::create(const GDLUser& userData) {
	auto ret = new UserInfoPopup();
	if (ret && ret->init(userData)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool UserInfoPopup::init(const GDLUser& userData) {
    if (!Popup::init(450.0f, 280.0f)) return false;

	m_userData = userData;

	m_userLoadListener = UserLoadedEvent(m_userData.id).listen(
		[this](Result<GDLUser, APIError> result) {
			if (result.isOk()) {
				drawUI();
			}
		}
	);

	GDL::API::Users::getUser(m_userData.id);

    return true;
}

void UserInfoPopup::drawUI() {
	auto fullUserData = GDL::Cache::Users::getUser(m_userData.id);
	if (!fullUserData || !fullUserData->isFull()) return;

	auto scrollBG = NineSlice::create("square02b_001.png");
	scrollBG->setColor({ 0, 0, 0 });
	scrollBG->setOpacity(51);
	scrollBG->setContentSize({ m_mainLayer->getContentSize() - ccp(40.0f, 40.0f) });
	m_mainLayer->addChildAtPosition(
		scrollBG,
		Anchor::Center,
		{}
	);

	auto scrollLayer = ScrollLayer::create(scrollBG->getContentSize());
	scrollLayer->m_contentLayer->setLayout(
		SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Center)
		->setMainAxisScaling(AxisScaling::Fit)
        ->setGap(5.0f)
        ->ignoreInvisibleChildren(false)
		->setPadding(Padding::uniform(10.0f))
	);
	scrollLayer->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChildAtPosition(
		scrollLayer,
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

	auto content = scrollLayer->m_contentLayer;

	auto basicInfoNode = CCNode::create();
	basicInfoNode->setContentSize({ m_mainLayer->getContentWidth() - 60.0f, 30.0f });
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

	m_basicInfoMenu = CCMenu::create();
	m_basicInfoMenu->setContentSize(basicInfoNode->getContentSize());
	basicInfoNode->addChildAtPosition(
		m_basicInfoMenu,
		Anchor::Center,
		{}
	);

	m_usernameLabel = CCLabelBMFont::create(
		fmt::format("{}", fullUserData->username).c_str(),
		"bigFont.fnt"
	);
	m_usernameLabel->setScale(0.65f);
	m_usernameLabel->setAnchorPoint({ 0.0f, 0.5f });
	m_basicInfoMenu->addChildAtPosition(
		m_usernameLabel,
		Anchor::Left,
		{ 10.0f, 0.0f }
	);

	auto badgeSprName = Utils::getBadgeSpriteName(fullUserData->badge);
    auto bagdeSpr = CCSprite::create(badgeSprName.c_str());
    bagdeSpr->setScale(20.0f / bagdeSpr->getContentHeight());

	m_badgeBtn = CCMenuItemExt::createSpriteExtra(
        bagdeSpr, [fullUserData](auto) {
            Utils::alert(
                "Badge Info",
                Utils::getBadgeInfo(fullUserData->badge)
            );
        }
    );
    m_badgeBtn->setAnchorPoint({ 0.5f, 0.5f });
    m_basicInfoMenu->addChildAtPosition(
		m_badgeBtn,
		Anchor::Right,
		{ -10.0f - m_badgeBtn->getContentWidth() / 2.0f, 0.0f }
	);

    auto flagSprName = Utils::getCountrySpriteName(fullUserData->country);
    auto flagSpr = CCSprite::create(flagSprName.c_str());
    flagSpr->setScale(20.0f / flagSpr->getContentHeight());

    m_flagBtn = CCMenuItemExt::createSpriteExtra(
        flagSpr, [fullUserData](auto) {
            auto countryName = string::replace(fullUserData->country, "-", " ");
            Utils::alert(
                "Country Info",
                fmt::format("Player's country: {}", countryName)
            );
        }
    );
    m_flagBtn->setAnchorPoint({ 0.5f, 0.5f });
    m_basicInfoMenu->addChildAtPosition(
		m_flagBtn,
		Anchor::Right,
		{ -20.0f - m_badgeBtn->getContentWidth() - m_flagBtn->getContentWidth() / 2.0f, 0.0f }
	);


	auto statsMenu = CCNode::create();
	statsMenu->setLayout(
		RowLayout::create()
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false)
	);
	statsMenu->setContentSize({ m_mainLayer->getContentWidth() - 60.0f, 40.0f });
	statsMenu->setAnchorPoint({ 0.5f, 0.5f });
	content->addChild(statsMenu);

	auto rankNode = TailyUI::createStatNode(
		fmt::format("#{}", fullUserData->placement).c_str(),
		"Rank", "rank-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 5.0f) / 2.0f
	);
	statsMenu->addChild(rankNode);

	auto scoreNode = TailyUI::createStatNode(
		fmt::format("{:.2f}", fullUserData->points).c_str(),
		"Score", "score-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 5.0f) / 2.0f
	);
	statsMenu->addChild(scoreNode);

	statsMenu->updateLayout();

	auto hardestNode = TailyUI::createStatNode(
		fmt::format("#{} {}", fullUserData->hardest->placement, fullUserData->hardest->name).c_str(),
		"Hardest", "hardest-icon.png"_spr,
		m_mainLayer->getContentWidth() - 60.0f
	);
	content->addChild(hardestNode);

	if (fullUserData->mainList.has_value() && !fullUserData->mainList->empty()) {
		auto mainLevelsNode = TailyUI::createUserLevelsNode(
			fullUserData->mainList,
			"Main levels",
			"main-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(mainLevelsNode);
	}

	if (fullUserData->extendedList.has_value() && !fullUserData->extendedList->empty()) {
		auto extendedLevelsNode = TailyUI::createUserLevelsNode(
			fullUserData->extendedList,
			"Extended levels",
			"extended-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(extendedLevelsNode);
	}

	if (fullUserData->advancedList.has_value() && !fullUserData->advancedList->empty()) {
		auto advancedLevelsNode = TailyUI::createUserLevelsNode(
			fullUserData->advancedList,
			"Advanced levels",
			"advanced-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(advancedLevelsNode);
	}

	if (fullUserData->unboundedList.has_value() && !fullUserData->unboundedList->empty()) {
		auto unboundedLevelsNode = TailyUI::createUserLevelsNode(
			fullUserData->unboundedList,
			"Unbounded levels",
			"unbounded-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(unboundedLevelsNode);
	}

	if (fullUserData->verifiedList.has_value() && !fullUserData->verifiedList->empty()) {
		auto verifiedLevelsNode = TailyUI::createUserLevelsNode(
			fullUserData->verifiedList,
			"Verified levels",
			"verified-levels-icon.png"_spr,
			m_mainLayer->getContentWidth() - 60.0f
		);
		content->addChild(verifiedLevelsNode);
	}


	content->updateLayout();
	scrollLayer->scrollToTop();
}