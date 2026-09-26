#include "nodes.hpp"
#include "../Popups/UserInfoPopup/UserInfoPopup.hpp"

using namespace geode::prelude;

namespace TailyUI {
    CCNode* createStatNode(const std::string& text, const std::string& subtext, const std::string& icon, float width) {
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

    CCNode* createUserLevelsNode(const OptGDLBasicLevels& levels, const std::string& text, const std::string& icon, float width) {
        auto node = CCNode::create();
        node->setContentWidth(width);
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setScale(0.5f);

        auto iconSpr = CCSprite::create(icon.c_str());
        iconSpr->setScale(12.0f / iconSpr->getContentWidth());

        auto textLabel = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        textLabel->setScale(0.3f);
        textLabel->setAnchorPoint({ 0.0f, 0.5f });

        auto countLabel = CCLabelBMFont::create(
            fmt::format("{}", levels->size()).c_str(),
            "bigFont.fnt"
        );
        countLabel->setScale(0.3f);
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

            auto levelBtn = cocos::CCMenuItemExt::createSpriteExtra(
                levelBtnSpr, [level](auto) {
                    web::openLinkInBrowser(level.videoURL);
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

    CCNode* createPlayerListNode(const std::vector<GDLCountryUser>& users, float width) {
        auto node = CCNode::create();
        node->setContentWidth(width);
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setScale(0.5f);

        auto iconSpr = CCSprite::create("blue-players-icon.png"_spr);
        iconSpr->setScale(12.0f / iconSpr->getContentWidth());

        auto textLabel = CCLabelBMFont::create("Players", "bigFont.fnt");
        textLabel->setScale(0.3f);
        textLabel->setAnchorPoint({ 0.0f, 0.5f });

        auto countLabel = CCLabelBMFont::create(
            fmt::format("{}", users.size()).c_str(),
            "bigFont.fnt"
        );
        countLabel->setScale(0.3f);
        countLabel->setAnchorPoint({ 1.0f, 0.5f });

        auto listNode = CCNode::create();
        listNode->setContentSize({ node->getContentWidth(), 0.0f });
        listNode->setAnchorPoint({ 0.5f, 1.0f });
        listNode->setLayout(
            SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Center)
            ->setMainAxisScaling(AxisScaling::Fit)
            ->setGap(2.0f)
            ->ignoreInvisibleChildren(true)
            ->setPadding({ 7.5f, 0, 7.5f, 7.5f })
        );

        int placementCounter = 0;
        for (const auto& user : users) {
            placementCounter++;
            auto playerListCell = createPlayerListCell(user, placementCounter, node->getContentWidth() - 15.0f);
            listNode->addChild(playerListCell);
        }
        listNode->updateLayout();

        node->setContentHeight(listNode->getContentHeight() + 20.0f);
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
            listNode,
            Anchor::Top,
            { 0.0f, -20.0f }
        );

        return node;
    }

    CCNode* createLevelButtonSprite(const GDLBasicLevel& level) {
        auto text =
        level.percent.has_value() ?
        fmt::format("{} {}%", level.name, level.percent.value()) :
        level.name;

        auto levelNameLabel = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
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

    CCNode* createPlayerListCell(const GDLCountryUser& user, int placement, float width) {
        auto node = CCNode::create();
        node->setContentSize({ width, 30.0f });
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

        auto menu = CCMenu::create();
        menu->setContentSize(node->getContentSize());
        menu->setAnchorPoint({ 0.5f, 0.5f });
        menu->ignoreAnchorPointForPosition(false);
        node->addChildAtPosition(
            menu,
            Anchor::Center,
            {}
        );

        auto placementLabel = CCLabelBMFont::create(
            fmt::format("#{}", placement).c_str(), "bigFont.fnt"
        );
        placementLabel->setScale(0.5f);
        placementLabel->setAnchorPoint({ 0.5f, 0.5f });
        node->addChildAtPosition(
            placementLabel,
            Anchor::Left,
            { 10.0f + placementLabel->getScaledContentWidth() / 2.0f, 0.0f }
        );

        auto usernameLabel = CCLabelBMFont::create(
            user.username.c_str(), "bigFont.fnt"
        );
        usernameLabel->setScale(0.5f);

        auto usernameBtn = CCMenuItemExt::createSpriteExtra(
            usernameLabel, [user](auto) {
                UserInfoPopup::create(user.id)->show();
            }
        );
        usernameBtn->setPosition(placementLabel->getPosition() + ccp(placementLabel->getScaledContentWidth() / 2.0f + usernameBtn->getContentWidth() / 2.0f + 10.0f, 0.0f));
        menu->addChild(usernameBtn);

        auto pointsLabel = CCLabelBMFont::create(
            fmt::format("{:.2f}", user.points).c_str(), "bigFont.fnt"
        );
        pointsLabel->setScale(0.5f);
        pointsLabel->setColor({ 0, 212, 255 });
        pointsLabel->setAnchorPoint({ 0.5f, 0.5f });
        node->addChildAtPosition(
            pointsLabel,
            Anchor::Right,
            { -pointsLabel->getScaledContentWidth() / 2.0f - 10.0f, 0.0f }
        );

        return node;
    }
}