#include "UserInfoPopup.hpp"
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

	auto rankNode = createStatNode(
		fmt::format("#{}", fullUserData->placement).c_str(),
		"Rank", "rank-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 5.0f) / 2.0f
	);
	statsMenu->addChild(rankNode);

	auto scoreNode = createStatNode(
		fmt::format("{:.2f}", fullUserData->points).c_str(),
		"Score", "score-icon.png"_spr,
		(m_mainLayer->getContentWidth() - 60.0f - 5.0f) / 2.0f
	);
	statsMenu->addChild(scoreNode);

	statsMenu->updateLayout();

	auto hardestNode = createStatNode(
		fmt::format("#{} {}", fullUserData->hardest->placement, fullUserData->hardest->name).c_str(),
		"Hardest", "hardest-icon.png"_spr,
		m_mainLayer->getContentWidth() - 60.0f
	);
	content->addChild(hardestNode);

	if (fullUserData->mainList.has_value() && !fullUserData->mainList->empty()) {
		auto mainLevelsNode = createUserLevelsNode(
			fullUserData->mainList,
			"Main levels",
			"main-levels-icon.png"_spr
		);
		content->addChild(mainLevelsNode);
	}

	if (fullUserData->extendedList.has_value() && !fullUserData->extendedList->empty()) {
		auto extendedLevelsNode = createUserLevelsNode(
			fullUserData->extendedList,
			"Extended levels",
			"extended-levels-icon.png"_spr
		);
		content->addChild(extendedLevelsNode);
	}

	if (fullUserData->advancedList.has_value() && !fullUserData->advancedList->empty()) {
		auto advancedLevelsNode = createUserLevelsNode(
			fullUserData->advancedList,
			"Advanced levels",
			"advanced-levels-icon.png"_spr
		);
		content->addChild(advancedLevelsNode);
	}

	if (fullUserData->unboundedList.has_value() && !fullUserData->unboundedList->empty()) {
		auto unboundedLevelsNode = createUserLevelsNode(
			fullUserData->unboundedList,
			"Unbounded levels",
			"unbounded-levels-icon.png"_spr
		);
		content->addChild(unboundedLevelsNode);
	}

	if (fullUserData->verifiedList.has_value() && !fullUserData->verifiedList->empty()) {
		auto verifiedLevelsNode = createUserLevelsNode(
			fullUserData->verifiedList,
			"Verified levels",
			"verified-levels-icon.png"_spr
		);
		content->addChild(verifiedLevelsNode);
	}


	content->updateLayout();
	scrollLayer->scrollToTop();
}

CCNode* UserInfoPopup::createStatNode(const std::string& text, const std::string& subtext, const std::string& icon, float width) {
	auto node = CCNode::create();
	node->setContentSize({ width, 40.0f });
	node->setAnchorPoint({ 0.5f, 0.5f });

	auto bg = NineSlice::create("square02b_001.png");
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(51);
	bg->setContentSize(node->getContentSize() * 2.0f);
	bg->setScale(0.5f);
	node->addChildAtPosition(
		bg,
		Anchor::Center,
		{}
	);

	auto iconBG = NineSlice::create("square02b_001.png");
	iconBG->setColor({ 0, 0, 0 });
	iconBG->setOpacity(51);
	iconBG->setContentSize({ 50.0f, 50.0f });
	iconBG->setScale(0.5f);
	node->addChildAtPosition(
		iconBG,
		Anchor::Left,
		{ node->getContentHeight() / 2.0f, 0.0f }
	);

	auto iconSpr = CCSprite::create(icon.c_str());
	iconSpr->setScale(16.0f / iconSpr->getContentWidth());
	node->addChildAtPosition(
		iconSpr,
		Anchor::Left,
		{ node->getContentHeight() / 2.0f, 0.0f }
	);

	auto textLabel = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
	textLabel->setScale(0.35f);
	textLabel->setAnchorPoint({ 0.0f, 1.0f });
	node->addChildAtPosition(
		textLabel,
		Anchor::Left,
		{ (node->getContentHeight() - iconBG->getScaledContentHeight()) / 2.0f + iconBG->getScaledContentWidth() + 5.0f, 10.0f }
	);

	auto subtextLabel = CCLabelBMFont::create(subtext.c_str(), "chatFont.fnt");
	subtextLabel->setScale(0.4f);
	subtextLabel->setAnchorPoint({ 0.0f, 0.0f });
	node->addChildAtPosition(
		subtextLabel,
		Anchor::Left,
		{ (node->getContentHeight() - iconBG->getScaledContentHeight()) / 2.0f + iconBG->getScaledContentWidth() + 5.0f, -10.0f }
	);

	return node;
}

CCNode* UserInfoPopup::createUserLevelsNode(const OptGDLBasicLevels& levels, const std::string& text, const std::string& icon) {
	auto node = CCNode::create();
	node->setContentWidth(m_mainLayer->getContentWidth() - 60.0f);
	node->setAnchorPoint({ 0.5f, 0.5f });

	auto bg = NineSlice::create("square02b_001.png");
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(51);
	bg->setScale(0.5f);

	auto iconSpr = CCSprite::create(icon.c_str());
	iconSpr->setScale(12.0f / iconSpr->getContentWidth());

	auto textLabel = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
	textLabel->setScale(0.25f);
	textLabel->setAnchorPoint({ 0.0f, 0.5f });

	auto countLabel = CCLabelBMFont::create(
		fmt::format("{}", levels->size()).c_str(),
		"bigFont.fnt"
	);
	countLabel->setScale(0.25f);
	countLabel->setAnchorPoint({ 1.0f, 0.5f });

	auto levelsMenu = CCMenu::create();
	levelsMenu->setContentSize({ node->getContentWidth(), 0.0f });
	levelsMenu->setAnchorPoint({ 0.5f, 1.0f });
	levelsMenu->setLayout(
		RowLayout::create()
		->setGap(3.0f)
		->setAxisAlignment(AxisAlignment::Start)
		->setGrowCrossAxis(true)
		->setCrossAxisOverflow(true)
		->setPadding({ 7.5f, 0, 7.5f, 7.5f })
	);

	for (const auto& level : *levels) {
		auto levelBtnSpr = createLevelButtonSprite(level);

		auto levelBtn = CCMenuItemExt::createSpriteExtra(
			levelBtnSpr, [level](auto) {
				utils::web::openLinkInBrowser(level.videoURL);
			}
		);
		levelsMenu->addChild(levelBtn);
	}
	levelsMenu->updateLayout();

	node->setContentHeight(levelsMenu->getContentHeight() + 20.0f);
	bg->setContentSize(node->getContentSize() * 2.0f);

	node->addChildAtPosition(
		bg,
		Anchor::Center,
		{}
	);
	node->addChildAtPosition(
		iconSpr,
		Anchor::TopLeft,
		{ 7.5f + iconSpr->getScaledContentWidth() / 2.0f, -10.0f }
	);
	node->addChildAtPosition(
		textLabel,
		Anchor::TopLeft,
		{ iconSpr->getPositionX() + iconSpr->getScaledContentWidth() / 2.0f + 3.0f, -10.0f }
	);
	node->addChildAtPosition(
		countLabel,
		Anchor::TopRight,
		{ -7.5f, -10.0f }
	);
	node->addChildAtPosition(
		levelsMenu,
		Anchor::Top,
		{ 0.0f, -20.0f }
	);

	return node;
}

CCNode* UserInfoPopup::createLevelButtonSprite(const GDLBasicLevel& level) {
	auto levelNameLabel = CCLabelBMFont::create(level.name.c_str(), "chatFont.fnt");
	levelNameLabel->setScale(0.5f);
	levelNameLabel->setAnchorPoint({ 0.5f, 0.5f });

	auto node = CCNode::create();
	node->setContentSize(levelNameLabel->getScaledContentSize() + ccp(8.0f, 4.0f));
	node->setAnchorPoint({ 0.5f, 0.5f });

	auto bg = NineSlice::create("square02b_001.png");
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(51);
	bg->setContentSize(node->getContentSize() * 4.0f);
	bg->setScale(0.25f);
	node->addChildAtPosition(
		bg,
		Anchor::Center,
		{}
	);

	node->addChildAtPosition(
		levelNameLabel,
		Anchor::Center,
		{}
	);

	return node;
}