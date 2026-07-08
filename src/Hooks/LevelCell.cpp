#include <Geode/modify/LevelCell.hpp>
#include "../API/Levels/Levels.hpp"
#include "../Utils/RemovePlacement.hpp"

class $modify(MyLevelCell, LevelCell) {
    struct Fields {
        ListenerHandle m_listener;
        std::unordered_map<CCNode*, float> m_origPositions;
    };

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);
        if (
            !level || level->m_levelType == GJLevelType::Main || level->m_levelType == GJLevelType::Editor ||
            GDL::Cache::Levels::isLevelWOPlacement(level->m_levelID.value())
        ) return;

        bool isExtremeDemon =
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::ExtremeDemon) ||
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::InsaneDemon);

        if (isExtremeDemon || level->m_stars == 0) {
            auto downloadsIcon = m_mainLayer->getChildByID("downloads-icon");
            auto downloadsLabel = m_mainLayer->getChildByID("downloads-label");
            auto likesIcon = m_mainLayer->getChildByID("likes-icon");
            auto likesLabel = m_mainLayer->getChildByID("likes-label");
            auto orbsIcon = m_mainLayer->getChildByID("orbs-icon");
            auto orbsLabel = m_mainLayer->getChildByID("orbs-label");

            if ( !downloadsIcon || !downloadsLabel || !likesIcon || !likesLabel ) return;

            float likesLabelPos = likesLabel->getPositionX() + likesLabel->getScaledContentWidth();
            float likesIconPos = likesIcon->getPositionX() - likesIcon->getScaledContentWidth() / 2.0f;
            float downloadsLabelPos = downloadsLabel->getPositionX() + downloadsLabel->getScaledContentWidth();
            
            float gdlIconX = (orbsLabel
                ? orbsLabel->getPositionX() + orbsLabel->getScaledContentWidth() + (likesIconPos - downloadsLabelPos)
                : likesLabelPos + (likesIconPos - downloadsLabelPos))
                + (m_compactView ? 9.2f : 13.8f) / 2.0f;

            auto gdlIcon = CCSprite::create("global-list.png"_spr);
            gdlIcon->setScale((m_compactView ? 9.2f : 13.8f) / gdlIcon->getContentWidth());
            gdlIcon->setPosition({ gdlIconX, m_compactView ? 8.5f : 14.0f });
            gdlIcon->setID("gdl-icon"_spr);
            m_mainLayer->addChild(gdlIcon);

            float gdlLabelX = gdlIconX + gdlIcon->getScaledContentWidth() / 2.0f + (m_compactView ? 5.4f : 3.1f);

            auto gdlLabel = CCLabelBMFont::create("...", "bigFont.fnt");
            gdlLabel->setScale(m_compactView ? 0.3 : 0.4f);
            gdlLabel->setAnchorPoint({ 0.0f, 0.5f });
            gdlLabel->setPosition({ gdlLabelX, m_compactView ? 8.5f : 14.0f });
            gdlLabel->setID("gdl-label"_spr);
            m_mainLayer->addChild(gdlLabel);

            std::unordered_map<CCNode*, float>& origPositions = m_fields->m_origPositions;

            if (gdlLabel->getPositionX() + 50.0f > 350.0f) {
                float gap = ((gdlLabel->getPositionX() + 50.0f) - 350.0f) / (orbsIcon ? 4.0f : 3.0f);

                origPositions[downloadsIcon] = downloadsIcon->getPositionX();
                origPositions[downloadsLabel] = downloadsLabel->getPositionX();
                origPositions[likesIcon] = likesIcon->getPositionX();
                origPositions[likesLabel] = likesLabel->getPositionX();
                origPositions[gdlIcon] = gdlIcon->getPositionX();
                origPositions[gdlLabel] = gdlLabel->getPositionX();
                if (orbsIcon) origPositions[orbsIcon] = orbsIcon->getPositionX();
                if (orbsLabel) origPositions[orbsLabel] = orbsLabel->getPositionX();

                downloadsIcon->setPositionX(downloadsIcon->getPositionX() - gap);
                downloadsLabel->setPositionX(downloadsLabel->getPositionX() - gap);
                likesIcon->setPositionX(likesIcon->getPositionX() - gap * 2.0f);
                likesLabel->setPositionX(likesLabel->getPositionX() - gap * 2.0f);
                gdlIcon->setPositionX(gdlIcon->getPositionX() - gap * (orbsIcon ? 4.0f : 3.0f));
                gdlLabel->setPositionX(gdlLabel->getPositionX() - gap * (orbsIcon ? 4.0f : 3.0f));
                if (orbsIcon) orbsIcon->setPositionX(orbsIcon->getPositionX() - gap * 3.0f);
                if (orbsLabel) orbsLabel->setPositionX(orbsLabel->getPositionX() - gap * 3.0f);
            }

            m_fields->m_listener = LevelLoadedEvent(level->m_levelID.value()).listen(
                [this](Result<GDLLevel*, APIError> result) {
                    if (!this || !m_mainLayer) return;

                    auto gdlLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("gdl-label"_spr));
                    auto gdlIcon = m_mainLayer->getChildByID("gdl-icon"_spr);
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

            GDL::API::getLevel(level->m_levelID.value());
        }
    }
};