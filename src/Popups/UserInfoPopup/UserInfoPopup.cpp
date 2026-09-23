#include "UserInfoPopup.hpp"
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

	auto basicInfoBG = CCScale9Sprite::create("square02b_001.png");
	basicInfoBG->setColor({ 123, 68, 41 });
	basicInfoBG->setContentSize({ (m_mainLayer->getContentWidth() - 40.0f) * 2, 30.0f * 2 });
	basicInfoBG->setScale(0.5f);
	m_mainLayer->addChildAtPosition(
		basicInfoBG,
		Anchor::Top,
		{ 0.0f, -basicInfoBG->getScaledContentHeight() / 2.0f - 20.0f }
	);

	m_basicInfoMenu = CCMenu::create();
	m_basicInfoMenu->setContentSize({ m_mainLayer->getContentWidth() - 40.0f, 30.0f });
	m_mainLayer->addChildAtPosition(
		m_basicInfoMenu,
		Anchor::Top,
		{ 0.0f, -m_basicInfoMenu->getScaledContentHeight() / 2.0f - 20.0f }
	);

	m_usernameLabel = CCLabelBMFont::create(
		fmt::format("{}. {}", userData.placement, userData.username).c_str(),
		"bigFont.fnt"
	);
	m_usernameLabel->setScale(0.65f);
	m_usernameLabel->setAnchorPoint({ 0.0f, 0.5f });
	m_basicInfoMenu->addChildAtPosition(
		m_usernameLabel,
		Anchor::Left,
		{ 10.0f, 0.0f }
	);

	auto badgeSprName = Utils::getBadgeSpriteName(userData.badge);
    auto bagdeSpr = CCSprite::create(badgeSprName.c_str());
    bagdeSpr->setScale(20.0f / bagdeSpr->getContentHeight());

	m_badgeBtn = CCMenuItemExt::createSpriteExtra(
        bagdeSpr, [userData](auto) {
            Utils::alert(
                "Badge Info",
                Utils::getBadgeInfo(userData.badge)
            );
        }
    );
    m_badgeBtn->setAnchorPoint({ 0.5f, 0.5f });
    m_basicInfoMenu->addChildAtPosition(
		m_badgeBtn,
		Anchor::Right,
		{ -10.0f - m_badgeBtn->getContentWidth() / 2.0f, 0.0f }
	);

    auto flagSprName = Utils::getCountrySpriteName(userData.country);
    auto flagSpr = CCSprite::create(flagSprName.c_str());
    flagSpr->setScale(20.0f / flagSpr->getContentHeight());

    m_flagBtn = CCMenuItemExt::createSpriteExtra(
        flagSpr, [userData](auto) {
            auto countryName = string::replace(userData.country, "-", " ");
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


	auto statsBGSection = CCNode::create();
	statsBGSection->setLayout(
		RowLayout::create()
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false)
	);
	statsBGSection->setContentSize({ m_mainLayer->getContentWidth() - 40.0f, 50.0f });
	statsBGSection->setAnchorPoint({ 0.5f, 0.5f });
	m_mainLayer->addChildAtPosition(
		statsBGSection,
		Anchor::Top,
		{ 0.0f, -basicInfoBG->getScaledContentHeight() - statsBGSection->getContentHeight() / 2.0f - 30.0f }
	);

	for (int i = 0; i < 3; i++) {
		auto statBG = CCScale9Sprite::create("square02b_001.png");
		statBG->setColor({ 123, 68, 41 });
		statBG->setContentSize({ (m_mainLayer->getContentWidth() - 60.0f) * 2 / 3, 50.0f * 2 });
		statBG->setScale(0.5f);
		statsBGSection->addChild(statBG);
	}
	statsBGSection->updateLayout();

    return true;
}