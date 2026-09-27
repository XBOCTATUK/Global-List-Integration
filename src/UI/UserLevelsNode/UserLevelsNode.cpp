#include "UserLevelsNode.hpp"

using namespace geode::prelude;

namespace TailyUI {
    UserLevelsNode* UserLevelsNode::create(
        const OptGDLBasicLevels& levels, const std::string& text,
        const std::string& icon, float width
    ) {
    	auto ret = new UserLevelsNode();
        if (ret && ret->init(levels, text, icon, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool UserLevelsNode::init(
        const OptGDLBasicLevels& levels, const std::string& text,
        const std::string& icon, float width
    ) {
        if (!CCNode::init()) return false;

        setContentWidth(width);
        setAnchorPoint({ 0.5f, 0.5f });

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setScale(0.5f);
        bg->setID("background");

        m_icon = CCSprite::create(icon.c_str());
        m_icon->setScale(12.0f / m_icon->getContentWidth());
        m_icon->setID("icon");

        m_textLabel = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        m_textLabel->setScale(0.35f);
        m_textLabel->setAnchorPoint({ 0.0f, 0.5f });
        m_textLabel->setID("list-title");

        m_countLabel = CCLabelBMFont::create(
            fmt::format("{}", levels->size()).c_str(),
            "bigFont.fnt"
        );
        m_countLabel->setScale(0.35f);
        m_countLabel->setAnchorPoint({ 1.0f, 0.5f });
        m_countLabel->setID("count-text");

        m_levelsMenu = CCMenu::create();
        m_levelsMenu->setContentSize({ getContentWidth(), 0.0f });
        m_levelsMenu->setAnchorPoint({ 0.5f, 1.0f });
        m_levelsMenu->setLayout(
            RowLayout::create()
            ->setGap(3.0f)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
            ->setPadding({ 7.5f, 0, 7.5f, 7.5f })
        );
        m_levelsMenu->setID("level-menu");

        for (const auto& level : *levels) {
            auto levelBtnSpr = createLevelButtonSprite(level);

            auto levelBtn = cocos::CCMenuItemExt::createSpriteExtra(
                levelBtnSpr, [level](auto) {
                    web::openLinkInBrowser(level.videoURL);
                }
            );
            m_levelsMenu->addChild(levelBtn);
        }
        m_levelsMenu->updateLayout();

        setContentHeight(m_levelsMenu->getContentHeight() + 20.0f);
        bg->setContentSize(getContentSize() * 2.0f);

        addChildAtPosition(
            bg,
            Anchor::Center,
            {}
        );
        addChildAtPosition(
            m_icon,
            Anchor::TopLeft,
            { 7.5f + m_icon->getScaledContentWidth() / 2.0f, -10.0f }
        );
        addChildAtPosition(
            m_textLabel,
            Anchor::TopLeft,
            { m_icon->getPositionX() + m_icon->getScaledContentWidth() / 2.0f + 3.0f, -10.0f }
        );
        addChildAtPosition(
            m_countLabel,
            Anchor::TopRight,
            { -7.5f, -10.0f }
        );
        addChildAtPosition(
            m_levelsMenu,
            Anchor::Top,
            { 0.0f, -20.0f }
        );

        return true;
    }

    CCNode* UserLevelsNode::createLevelButtonSprite(const GDLBasicLevel& level) {
        auto text =
        level.percent.has_value() ?
        fmt::format("{} {}%", level.name, level.percent.value()) :
        level.name;

        auto levelNameLabel = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
        levelNameLabel->setScale(0.6f);
        levelNameLabel->setAnchorPoint({ 0.5f, 0.5f });
        levelNameLabel->setID("level-name");

        auto node = CCNode::create();
        node->setContentSize(levelNameLabel->getScaledContentSize() + ccp(8.0f, 4.0f));
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setContentSize(node->getContentSize() * 4.0f);
        bg->setScale(0.25f);
        bg->setID("background");
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
}