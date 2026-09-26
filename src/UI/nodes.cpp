#include "nodes.hpp"

namespace TailyUI {
    cocos2d::CCNode* createStatNode(const std::string& text, const std::string& subtext, const std::string& icon, float width) {
        auto node = cocos2d::CCNode::create();
        node->setContentSize({ width, 40.0f });
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = geode::NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setContentSize(node->getContentSize() * 2.0f);
        bg->setScale(0.5f);
        node->addChildAtPosition(
            bg,
            geode::Anchor::Center,
            {}
        );

        auto iconBG = geode::NineSlice::create("square02b_001.png");
        iconBG->setColor({ 0, 0, 0 });
        iconBG->setOpacity(51);
        iconBG->setContentSize({ 50.0f, 50.0f });
        iconBG->setScale(0.5f);
        node->addChildAtPosition(
            iconBG,
            geode::Anchor::Left,
            { node->getContentHeight() / 2.0f, 0.0f }
        );

        auto iconSpr = cocos2d::CCSprite::create(icon.c_str());
        iconSpr->setScale(16.0f / iconSpr->getContentWidth());
        node->addChildAtPosition(
            iconSpr,
            geode::Anchor::Left,
            { node->getContentHeight() / 2.0f, 0.0f }
        );

        auto textLabel = cocos2d::CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        textLabel->setScale(0.35f);
        textLabel->setAnchorPoint({ 0.0f, 1.0f });
        node->addChildAtPosition(
            textLabel,
            geode::Anchor::Left,
            { (node->getContentHeight() - iconBG->getScaledContentHeight()) / 2.0f + iconBG->getScaledContentWidth() + 5.0f, 10.0f }
        );

        auto subtextLabel = cocos2d::CCLabelBMFont::create(subtext.c_str(), "chatFont.fnt");
        subtextLabel->setScale(0.4f);
        subtextLabel->setAnchorPoint({ 0.0f, 0.0f });
        node->addChildAtPosition(
            subtextLabel,
            geode::Anchor::Left,
            { (node->getContentHeight() - iconBG->getScaledContentHeight()) / 2.0f + iconBG->getScaledContentWidth() + 5.0f, -10.0f }
        );

        return node;
    }

    cocos2d::CCNode* createUserLevelsNode(const OptGDLBasicLevels& levels, const std::string& text, const std::string& icon, float width) {
        auto node = cocos2d::CCNode::create();
        node->setContentWidth(width);
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = geode::NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setScale(0.5f);

        auto iconSpr = cocos2d::CCSprite::create(icon.c_str());
        iconSpr->setScale(12.0f / iconSpr->getContentWidth());

        auto textLabel = cocos2d::CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        textLabel->setScale(0.25f);
        textLabel->setAnchorPoint({ 0.0f, 0.5f });

        auto countLabel = cocos2d::CCLabelBMFont::create(
            fmt::format("{}", levels->size()).c_str(),
            "bigFont.fnt"
        );
        countLabel->setScale(0.25f);
        countLabel->setAnchorPoint({ 1.0f, 0.5f });

        auto levelsMenu = cocos2d::CCMenu::create();
        levelsMenu->setContentSize({ node->getContentWidth(), 0.0f });
        levelsMenu->setAnchorPoint({ 0.5f, 1.0f });
        levelsMenu->setLayout(
            geode::RowLayout::create()
            ->setGap(3.0f)
            ->setAxisAlignment(geode::AxisAlignment::Start)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
            ->setPadding({ 7.5f, 0, 7.5f, 7.5f })
        );

        for (const auto& level : *levels) {
            auto levelBtnSpr = createLevelButtonSprite(level);

            auto levelBtn = geode::cocos::CCMenuItemExt::createSpriteExtra(
                levelBtnSpr, [level](auto) {
                    geode::utils::web::openLinkInBrowser(level.videoURL);
                }
            );
            levelsMenu->addChild(levelBtn);
        }
        levelsMenu->updateLayout();

        node->setContentHeight(levelsMenu->getContentHeight() + 20.0f);
        bg->setContentSize(node->getContentSize() * 2.0f);

        node->addChildAtPosition(
            bg,
            geode::Anchor::Center,
            {}
        );
        node->addChildAtPosition(
            iconSpr,
            geode::Anchor::TopLeft,
            { 7.5f + iconSpr->getScaledContentWidth() / 2.0f, -10.0f }
        );
        node->addChildAtPosition(
            textLabel,
            geode::Anchor::TopLeft,
            { iconSpr->getPositionX() + iconSpr->getScaledContentWidth() / 2.0f + 3.0f, -10.0f }
        );
        node->addChildAtPosition(
            countLabel,
            geode::Anchor::TopRight,
            { -7.5f, -10.0f }
        );
        node->addChildAtPosition(
            levelsMenu,
            geode::Anchor::Top,
            { 0.0f, -20.0f }
        );

        return node;
    }

    cocos2d::CCNode* createLevelButtonSprite(const GDLBasicLevel& level) {
        auto levelNameLabel = cocos2d::CCLabelBMFont::create(level.name.c_str(), "chatFont.fnt");
        levelNameLabel->setScale(0.5f);
        levelNameLabel->setAnchorPoint({ 0.5f, 0.5f });

        auto node = cocos2d::CCNode::create();
        node->setContentSize(levelNameLabel->getScaledContentSize() + ccp(8.0f, 4.0f));
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = geode::NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setContentSize(node->getContentSize() * 4.0f);
        bg->setScale(0.25f);
        node->addChildAtPosition(
            bg,
            geode::Anchor::Center,
            {}
        );

        node->addChildAtPosition(
            levelNameLabel,
            geode::Anchor::Center,
            {}
        );

        return node;
    }
}