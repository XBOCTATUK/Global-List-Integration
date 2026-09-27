#include <Geode/modify/LevelInfoLayer.hpp>
#include "../Cache/Levels/Levels.hpp"
#include "../Settings/Settings.hpp"

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        ListenerHandle m_levelLoadListener;
        std::unordered_map<CCNode*, float> m_origPositions;
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto gdlLevel = GDL::Cache::Levels::getLevel(level->m_levelID.value());
        if (
            !level || level->m_levelType == GJLevelType::Main || level->m_levelType == GJLevelType::Editor ||
            !gdlLevel || !Settings::shouldLoadPlacement()
        ) return true;

        auto downloadsIcon = getChildByID("downloads-icon");
        auto lengthIcon = getChildByID("length-icon");

        if (
            !downloadsIcon || !m_downloadsLabel || !lengthIcon || !m_lengthLabel ||
            !m_likesIcon || !m_likesLabel || !m_orbsIcon || !m_orbsLabel
        ) return true;

        auto& origPositions = m_fields->m_origPositions;

        origPositions[downloadsIcon] = downloadsIcon->getPositionY();
        origPositions[m_downloadsLabel] = m_downloadsLabel->getPositionY();
        origPositions[m_likesIcon] = m_likesIcon->getPositionY();
        origPositions[m_likesLabel] = m_likesLabel->getPositionY();
        origPositions[lengthIcon] = lengthIcon->getPositionY();
        origPositions[m_lengthLabel] = m_lengthLabel->getPositionY();
        origPositions[m_exactLengthLabel] = m_exactLengthLabel->getPositionY();
        origPositions[m_orbsIcon] = m_orbsIcon->getPositionY();
        origPositions[m_orbsLabel] = m_orbsLabel->getPositionY();

        downloadsIcon->setPositionY(downloadsIcon->getPositionY() + 14.0f - 4.0f);
        m_downloadsLabel->setPositionY(m_downloadsLabel->getPositionY() + 14.0f - 4.0f);
        m_likesIcon->setPositionY(m_likesIcon->getPositionY() + 14.0f - 2.0f);
        m_likesLabel->setPositionY(m_likesLabel->getPositionY() + 14.0f - 2.0f);
        lengthIcon->setPositionY(lengthIcon->getPositionY() + 14.0f);
        m_lengthLabel->setPositionY(m_lengthLabel->getPositionY() + 14.0f);
        m_exactLengthLabel->setPositionY(m_exactLengthLabel->getPositionY() + 14.0f);
        m_orbsIcon->setPositionY(m_orbsIcon->getPositionY() + 14.0f + 2.0f);
        m_orbsLabel->setPositionY(m_orbsLabel->getPositionY() + 14.0f + 2.0f);

        float gdlIconX = lengthIcon->getPositionX() + lengthIcon->getContentWidth() / 2.0f;
        float gdlIconY =
            m_orbsIcon->isVisible()
            ? m_orbsIcon->getPositionY() - (downloadsIcon->getPositionY() - m_likesIcon->getPositionY())
            : lengthIcon->getPositionY() - (m_likesIcon->getPositionY() - lengthIcon->getPositionY());

        auto gdlIcon = CCSprite::create("globalListIcon.png"_spr);
        gdlIcon->setScale(23.0f / gdlIcon->getContentWidth());
        gdlIcon->setPosition({ gdlIconX, gdlIconY });
        gdlIcon->setID("gdl-icon"_spr);
        addChild(gdlIcon);

        auto gdlLabel = CCLabelBMFont::create(
            fmt::format("#{}", gdlLevel->placement).c_str(),
            "bigFont.fnt"
        );
        gdlLabel->setScale(0.5f);
        gdlLabel->setAnchorPoint({ 0.0f, 0.5f });
        gdlLabel->setPosition({ m_lengthLabel->getPositionX(), gdlIconY });
        gdlLabel->setID("gdl-label"_spr);
        addChild(gdlLabel);
        

        return true;
    }
};