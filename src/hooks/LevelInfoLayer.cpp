#include <Geode/modify/LevelInfoLayer.hpp>
#include "../GlobalList/API/Levels.hpp"
#include "../Utils.hpp"

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        ListenerHandle m_listener;
        std::unordered_map<CCNode*, float> m_origPositions;
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        if (!level || level->m_levelType == GJLevelType::Main || level->m_levelType == GJLevelType::Editor || GlobalList::Cache::isLevelWoPlacement(level->m_levelID.value())) return true;

        bool isDemon =
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::ExtremeDemon) ||
            level->m_demonDifficulty == static_cast<int>(DemonDifficultyType::InsaneDemon);

        std::unordered_map<CCNode*, float>& origPositions = m_fields->m_origPositions;
        if (isDemon || level->m_stars == 0) {
            auto downloadIcon = this->getChildByID("downloads-icon");
            auto downloadLabel = this->m_downloadsLabel;
            auto likesIcon = this->m_likesIcon;
            auto likesLabel = this->m_likesLabel;
            auto lengthIcon = this->getChildByID("length-icon");
            auto lengthLabel = this->m_lengthLabel;
            auto exactLengthLabel = this->getChildByID("exact-length-label");
            auto orbIcon = this->m_orbsIcon;
            auto orbLabel = this->m_orbsLabel;

            origPositions[downloadIcon] = downloadIcon->getPositionY();
            origPositions[downloadLabel] = downloadLabel->getPositionY();
            origPositions[likesIcon] = likesIcon->getPositionY();
            origPositions[likesLabel] = likesLabel->getPositionY();
            origPositions[lengthIcon] = lengthIcon->getPositionY();
            origPositions[lengthLabel] = lengthLabel->getPositionY();
            origPositions[exactLengthLabel] = exactLengthLabel->getPositionY();
            origPositions[orbIcon] = orbIcon->getPositionY();
            origPositions[orbLabel] = orbLabel->getPositionY();
            downloadIcon->setPositionY(downloadIcon->getPositionY() + 14.0f - 4.0f);
            downloadLabel->setPositionY(downloadLabel->getPositionY() + 14.0f - 4.0f);
            likesIcon->setPositionY(likesIcon->getPositionY() + 14.0f - 2.0f);
            likesLabel->setPositionY(likesLabel->getPositionY() + 14.0f - 2.0f);
            lengthIcon->setPositionY(lengthIcon->getPositionY() + 14.0f);
            lengthLabel->setPositionY(lengthLabel->getPositionY() + 14.0f);
            if (exactLengthLabel) exactLengthLabel->setPositionY(exactLengthLabel->getPositionY() + 14.0f);
            orbIcon->setPositionY(orbIcon->getPositionY() + 14.0f  + 2.0f);
            orbLabel->setPositionY(orbLabel->getPositionY() + 14.0f  + 2.0f);

            float gdlIconY = 0.0f;
            if (orbIcon->isVisible()) gdlIconY = orbIcon->getPositionY() - (lengthIcon->getPositionY() - orbIcon->getPositionY()) + 4.0f;
            else gdlIconY = lengthIcon->getPositionY() - (likesIcon->getPositionY() - lengthIcon->getPositionY() + 4.0f);

            float gdlIconX = lengthIcon->getPositionX() + lengthIcon->getContentWidth() / 2.0f;

            auto gdlIcon = CCSprite::create("global-list.png"_spr);
            gdlIcon->setContentSize({ 23.5f, 23.5f });
            gdlIcon->setScale(0.85f);
            gdlIcon->setAnchorPoint({ 0.5f, 0.5f });
            gdlIcon->setPosition({ gdlIconX, gdlIconY });
            gdlIcon->refreshTextureRect();
            gdlIcon->setID("gdl-icon"_spr);
            this->addChild(gdlIcon);

            auto gdlLabel = CCLabelBMFont::create("...", "bigFont.fnt");
            gdlLabel->setScale(0.5f);
            gdlLabel->setAnchorPoint({ 0.0f, 0.5f });
            gdlLabel->setPosition({ lengthLabel->getPositionX(), gdlIconY });
            gdlLabel->setID("gdl-label"_spr);
            this->addChild(gdlLabel);

            GlobalList::API::getLevelPlacement(level->m_levelID.value());

            m_fields->m_listener = PositionEvent(level->m_levelID.value()).listen(
                [this](int placement) {
                    if (auto gdlLabel = static_cast<CCLabelBMFont*>(this->getChildByID("gdl-label"_spr))) {
                        auto gdlIcon = this->getChildByID("gdl-icon"_spr);

                        if (placement == -1) gdlLabel->setString("N/A");
                        else if (placement > 0) gdlLabel->setString(fmt::format("#{}", placement).c_str());
                        else Utils::removePlacement(m_level->m_levelID, gdlLabel, gdlIcon, m_fields->m_origPositions, false);
                    }
                }
            );
        }

        return true;
    }
};