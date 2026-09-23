#include "GDLLeaderboardCell.hpp"
#include "../../Popups/UserInfoPopup/UserInfoPopup.hpp"
#include "../../Popups/CountryInfoPopup/CountryInfoPopup.hpp"
#include "../../Utils/BadgeUtils.hpp"
#include "../../Utils/FlagUtils.hpp"
#include "../../Utils/Alert.hpp"

using namespace geode::prelude;

GDLLeaderboardCell* GDLLeaderboardCell::create(const GDLUser& userData) {
	auto ret = new GDLLeaderboardCell();
	if (ret->init(userData)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

GDLLeaderboardCell* GDLLeaderboardCell::create(const GDLCountry& countryData) {
	auto ret = new GDLLeaderboardCell();
	if (ret->init(countryData)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool GDLLeaderboardCell::init(const GDLUser& userData) {
    if (!CCNode::init()) return false;

    m_userData = userData;
    bool compactCellsEnabled = Mod::get()->getSettingValue<bool>("compact-cells");

    setContentSize({356.0f, (compactCellsEnabled ? 30.0f : 40.0f)});

    auto rightMenu = CCMenu::create();
    rightMenu->setContentSize(getContentSize());
    rightMenu->setPosition({ 0.0f, 0.0f });
    rightMenu->setAnchorPoint({ 0.0f, 0.0f });
    rightMenu->setZOrder(1);
    addChild(rightMenu);

    m_placementLabel = CCLabelBMFont::create(fmt::format("{}.", userData.placement).c_str(), "bigFont.fnt");
    m_placementLabel->setScale(0.5f);
    m_placementLabel->setPosition({ 15.0f, getContentHeight() / 2.0f });
    m_placementLabel->setAnchorPoint({ 0.0f, 0.5f });
    addChild(m_placementLabel);

    auto nameLabel = CCLabelBMFont::create(userData.username.c_str(), "bigFont.fnt");
    nameLabel->limitLabelWidth(125.0f, 0.5f, 0.3f);

    m_nameBtn = CCMenuItemExt::createSpriteExtra(
        nameLabel, [this](auto) {
            UserInfoPopup::create(m_userData)->show();
        }
    );
    m_nameBtn->setPosition({ m_placementLabel->getPositionX() + m_placementLabel->getScaledContentWidth() + m_nameBtn->getContentWidth() / 2.0f + 10.0f, getContentHeight() / 2.0f });
    m_nameBtn->setAnchorPoint({ 0.5f, 0.5f });
    rightMenu->addChild(m_nameBtn);

    auto badgeSprName = Utils::getBadgeSpriteName(userData.badge);
    auto bagdeSpr = CCSprite::create(badgeSprName.c_str());
    bagdeSpr->setScale((compactCellsEnabled ? 16.0f : 24.0f) / bagdeSpr->getContentHeight());

    m_badgeBtn = CCMenuItemExt::createSpriteExtra(
        bagdeSpr, [userData](auto) {
            Utils::alert(
                "Badge Info",
                Utils::getBadgeInfo(userData.badge)
            );
        }
    );
    m_badgeBtn->setAnchorPoint({ 0.5f, 0.5f });
    m_badgeBtn->setPosition({ getContentWidth() - 15.0f - m_badgeBtn->getContentWidth() / 2.0f, getContentHeight() / 2.0f });
    rightMenu->addChild(m_badgeBtn);

    auto flagSprName = Utils::getCountrySpriteName(userData.country);
    m_flagSpr = CCSprite::create(flagSprName.c_str());
    m_flagSpr->setScale((compactCellsEnabled ? 16.0f : 24.0f) / m_flagSpr->getContentHeight());

    m_flagBtn = CCMenuItemExt::createSpriteExtra(
        m_flagSpr, [userData](auto) {
            auto countryName = string::replace(userData.country, "-", " ");
            Utils::alert(
                "Country Info",
                fmt::format("Player's country: {}", countryName)
            );
        }
    );
    m_flagBtn->setAnchorPoint({ 0.5f, 0.5f });
    m_flagBtn->setPosition({ m_badgeBtn->getPositionX() - m_badgeBtn->getContentWidth() / 2.0f - m_flagBtn->getContentWidth() / 2.0f - 10.0f, getContentHeight() / 2.0f });
    rightMenu->addChild(m_flagBtn);

    m_pointsLabel = CCLabelBMFont::create(fmt::format("{:.2f}", userData.points).c_str(), "bigFont.fnt");
    m_pointsLabel->setScale(0.5f);
    m_pointsLabel->setColor({ 0, 212, 255 });
    m_pointsLabel->setAnchorPoint({ 1.0f, 0.5f });
    m_pointsLabel->setPosition({ m_flagBtn->getPositionX() - m_flagBtn->getContentWidth() / 2.0f - 10.0f, getContentHeight() / 2.0f });
    addChild(m_pointsLabel);

    rightMenu->updateLayout();

    return true;
}

bool GDLLeaderboardCell::init(const GDLCountry& countryData) {
    if (!CCNode::init()) return false;

    m_countryData = countryData;
    bool compactCellsEnabled = Mod::get()->getSettingValue<bool>("compact-cells");

    setContentSize({356.0f, (compactCellsEnabled ? 30.0f : 40.0f)});

    auto rightMenu = CCMenu::create();
    rightMenu->setContentSize(getContentSize());
    rightMenu->setPosition({ 0.0f, 0.0f });
    rightMenu->setAnchorPoint({ 0.0f, 0.0f });
    rightMenu->setZOrder(1);
    addChild(rightMenu);

    m_placementLabel = CCLabelBMFont::create(fmt::format("{}.", countryData.placement).c_str(), "bigFont.fnt");
    m_placementLabel->setScale(0.5f);
    m_placementLabel->setPosition({ 15.0f, getContentHeight() / 2.0f });
    m_placementLabel->setAnchorPoint({ 0.0f, 0.5f });
    addChild(m_placementLabel);

    auto flagSprName = Utils::getCountrySpriteName(countryData.title);
    m_flagSpr = CCSprite::create(flagSprName.c_str());
    m_flagSpr->setScale((compactCellsEnabled ? 16.0f : 24.0f) / m_flagSpr->getContentHeight());
    m_flagSpr->setPosition({ m_placementLabel->getPositionX() + m_placementLabel->getScaledContentWidth() + m_flagSpr->getScaledContentWidth() / 2.0f + 10.0f, getContentHeight() / 2.0f });
    addChild(m_flagSpr);

    auto countryName = string::replace(countryData.title, "-", " ");
    auto nameLabel = CCLabelBMFont::create(countryName.c_str(), "bigFont.fnt");
    nameLabel->limitLabelWidth(150.0f, 0.5f, 0.3f);

    m_nameBtn = CCMenuItemExt::createSpriteExtra(
        nameLabel, [this](auto) {
            CountryInfoPopup::create(m_countryData)->show();
        }
    );
    m_nameBtn->setPosition({ m_flagSpr->getPositionX() + m_flagSpr->getScaledContentWidth() / 2.0f + m_nameBtn->getContentWidth() / 2.0f + 10.0f, getContentHeight() / 2.0f });
    m_nameBtn->setAnchorPoint({ 0.5f, 0.5f });
    rightMenu->addChild(m_nameBtn);

    m_pointsLabel = CCLabelBMFont::create(fmt::format("{:.2f}", countryData.points).c_str(), "bigFont.fnt");
    m_pointsLabel->setScale(0.5f);
    m_pointsLabel->setColor({ 0, 212, 255 });
    m_pointsLabel->setAnchorPoint({ 1.0f, 0.5f });
    m_pointsLabel->setPosition({ getContentWidth() - 15.0f, getContentHeight() / 2.0f });
    addChild(m_pointsLabel);

    return true;
}