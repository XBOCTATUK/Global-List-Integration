#include <Geode/modify/LevelInfoLayer.hpp>
#include "../API/Levels/Levels.hpp"
#include "../Utils/RemovePlacement.hpp"

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        ListenerHandle m_listener;
        std::unordered_map<CCNode*, float> m_origPositions;
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        if (
            !level || level->m_levelType == GJLevelType::Main || level->m_levelType == GJLevelType::Editor ||
            GlobalList::Cache::Levels::isLevelWOPlacement(level->m_levelID.value())
        ) return true;

        bool isExtremeDemon =
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::ExtremeDemon) ||
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::InsaneDemon);

        std::unordered_map<CCNode*, float>& origPositions = m_fields->m_origPositions;
        if (isExtremeDemon || level->m_stars == 0) {
            auto downloadsIcon = getChildByID("downloads-icon");
            auto lengthIcon = getChildByID("length-icon");

            if (
                !downloadsIcon || !m_downloadsLabel || !lengthIcon || !m_lengthLabel ||
                !m_likesIcon || !m_likesLabel || !m_orbsIcon || !m_orbsLabel
            ) return true;

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
            float gdlIconY = m_orbsIcon->isVisible()
                ? m_orbsIcon->getPositionY() - (downloadsIcon->getPositionY() - m_likesIcon->getPositionY())
                : lengthIcon->getPositionY() - (m_likesIcon->getPositionY() - lengthIcon->getPositionY());

            auto gdlIcon = CCSprite::create("global-list.png"_spr);
            gdlIcon->setScale(23.0f / gdlIcon->getContentWidth());
            gdlIcon->setPosition({ gdlIconX, gdlIconY });
            gdlIcon->setID("gdl-icon"_spr);
            addChild(gdlIcon);

            auto gdlLabel = CCLabelBMFont::create("...", "bigFont.fnt");
            gdlLabel->setScale(0.5f);
            gdlLabel->setAnchorPoint({ 0.0f, 0.5f });
            gdlLabel->setPosition({ m_lengthLabel->getPositionX(), gdlIconY });
            gdlLabel->setID("gdl-label"_spr);
            addChild(gdlLabel);

            m_fields->m_listener = LevelLoadedEvent(level->m_levelID.value()).listen(
                [this](Result<GlobalListLevel*, APIError> result) {
                    if (!this) return;

                    auto gdlLabel = static_cast<CCLabelBMFont*>(getChildByID("gdl-label"_spr));
                    auto gdlIcon = getChildByID("gdl-icon"_spr);
                    if (result.isOk()) {
                        auto GDLLevel = result.unwrap();
                        if (gdlLabel && gdlIcon) {
                            gdlLabel->setString(fmt::format("#{}", GDLLevel->placement).c_str());
                        }
                    }
                    else if (gdlLabel && gdlIcon) {
                        auto error = result.err().value();
                        if (error.message == APIMessage::LevelNotFound) {
                            Utils::removePlacement(m_level->m_levelID, gdlLabel, gdlIcon, m_fields->m_origPositions, true);
                        }
                        else {
                            gdlLabel->setString("N/A");
                        }
                    }
                }
            );

            GlobalList::API::getLevel(level->m_levelID.value());
        }

        return true;
    }
};