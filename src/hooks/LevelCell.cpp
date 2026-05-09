#include <Geode/modify/LevelCell.hpp>
#include "../GlobalList/API/Levels.hpp"
#include "../Utils.hpp"

class $modify(MyLevelCell, LevelCell) {
    struct Fields {
        ListenerHandle m_listener;
        std::unordered_map<CCNode*, float> m_origPositions;
    };

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);
        if (!level || level->m_levelType == GJLevelType::Main || level->m_levelType == GJLevelType::Editor || GlobalList::Cache::isLevelWoPlacement(level->m_levelID.value())) return;

        auto levelCellMain = this->m_mainLayer;

        bool isDemon =
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::ExtremeDemon) ||
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::InsaneDemon);

        if (isDemon || level->m_stars == 0) {
            auto downloadIcon = levelCellMain->getChildByID("downloads-icon");
            auto downloadLabel = levelCellMain->getChildByID("downloads-label");
            auto likesIcon = levelCellMain->getChildByID("likes-icon");
            auto likesLabel = levelCellMain->getChildByID("likes-label");
            auto lengthIcon = levelCellMain->getChildByID("length-icon");
            auto lengthLabel = levelCellMain->getChildByID("length-label");
            auto orbIcon = levelCellMain->getChildByID("orbs-icon");
            auto orbLabel = levelCellMain->getChildByID("orbs-label");

            float gdlIconX = 0.0f;
            float downloadIconPos = downloadIcon->getPositionX() - downloadIcon->getContentWidth() * downloadIcon->getScaleX() / 2.0f;
            float lengthLabelPos = lengthLabel->getPositionX() + lengthLabel->getContentWidth() * lengthLabel->getScaleX();
            float likesLabelPos = likesLabel->getScaleX() * likesLabel->getContentWidth() + likesLabel->getPositionX();

            if (orbLabel) {
                float orbLabelPos = orbLabel->getScaleX() * orbLabel->getContentWidth() + orbLabel->getPositionX();
                gdlIconX = orbLabelPos + (downloadIconPos - lengthLabelPos);
            }
            else gdlIconX = likesLabelPos + (downloadIconPos - lengthLabelPos);

            auto gdlIcon = CCSprite::create("global-list.png"_spr);
            gdlIcon->setScale(0.55f);
            gdlIcon->setContentSize({ 23.5f, 23.5f });
            gdlIcon->setAnchorPoint({ 0.5f, 0.5f });
            gdlIcon->setPosition({ gdlIconX + gdlIcon->getContentWidth() * 0.45f / 2.0f, 14.0f });
            gdlIcon->refreshTextureRect();
            gdlIcon->setID("gdl-icon"_spr);
            levelCellMain->addChild(gdlIcon);

            float gdlLabelX = gdlIconX + gdlIcon->getContentWidth() / 2 + 3.0f;

            auto gdlLabel = CCLabelBMFont::create("...", "bigFont.fnt");
            gdlLabel->setScale(0.4f);
            gdlLabel->setAnchorPoint({ 0.0f, 0.5f });
            gdlLabel->setPosition({ gdlLabelX, 14.0f });
            gdlLabel->setID("gdl-label"_spr);
            levelCellMain->addChild(gdlLabel);

            std::unordered_map<CCNode*, float>& origPositions = m_fields->m_origPositions;

            if (this->m_compactView) {
                gdlIcon->setScale(0.3f);
                gdlIcon->setPositionY(8.5f);
                gdlLabel->setScale(0.3f);
                gdlLabel->setPosition(gdlLabel->getPositionX() - 2.0f, 8.5f);
            }
            else if (gdlLabel->getPositionX() + 50.0f > 356.0f) {
                float gap = ((gdlLabel->getPositionX() + 50.0f) - 356.0f) / (orbIcon ? 4.0f : 3.0f);

                origPositions[downloadIcon] = downloadIcon->getPositionX();
                origPositions[downloadLabel] = downloadLabel->getPositionX();
                origPositions[likesIcon] = likesIcon->getPositionX();
                origPositions[likesLabel] = likesLabel->getPositionX();
                if (orbIcon) origPositions[orbIcon] = orbIcon->getPositionX();
                if (orbLabel) origPositions[orbLabel] = orbLabel->getPositionX();
                origPositions[gdlIcon] = gdlIcon->getPositionX();
                origPositions[gdlLabel] = gdlLabel->getPositionX();
                downloadIcon->setPositionX(downloadIcon->getPositionX() - gap);
                downloadLabel->setPositionX(downloadLabel->getPositionX() - gap);
                likesIcon->setPositionX(likesIcon->getPositionX() - gap * 2.0f);
                likesLabel->setPositionX(likesLabel->getPositionX() - gap * 2.0f);
                if (orbIcon) orbIcon->setPositionX(orbIcon->getPositionX() - gap * 3.0f);
                if (orbLabel) orbLabel->setPositionX(orbLabel->getPositionX() - gap * 3.0f);
                gdlIcon->setPositionX(gdlIcon->getPositionX() - gap * (orbIcon ? 4.0f : 3.0f));
                gdlLabel->setPositionX(gdlLabel->getPositionX() - gap * (orbIcon ? 4.0f : 3.0f));
            }

            GlobalList::API::getLevelPlacement(level->m_levelID.value());

            m_fields->m_listener = PositionEvent(level->m_levelID.value()).listen(
                [this](int placement) {
                    log::info("if (!m_mainLayer) return;");
                    if (!m_mainLayer) return;

                    log::info("if (auto gdlLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID(\"gdl-label\"_spr)))");
                    if (auto gdlLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("gdl-label"_spr))) {
                        log::info("auto gdlIcon = m_mainLayer->getChildByID(\"gdl-icon\"_spr);");
                        auto gdlIcon = m_mainLayer->getChildByID("gdl-icon"_spr);

                        log::info("{}", placement);
                        if (placement == -1) gdlLabel->setString("N/A");
                        else if (placement > 0) gdlLabel->setString(fmt::format("#{}", placement).c_str());
                        else Utils::removePlacement(m_level->m_levelID, gdlLabel, gdlIcon, m_fields->m_origPositions, true);
                    }
                }
            );
        }
    }
};