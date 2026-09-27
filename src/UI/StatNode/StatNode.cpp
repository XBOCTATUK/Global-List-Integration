#include "StatNode.hpp"

using namespace geode::prelude;

namespace TailyUI {
    StatNode* StatNode::create(
        const std::string& text, const std::string& subtext,
        const std::string& icon, float width
    ) {
    	auto ret = new StatNode();
        if (ret && ret->init(text, subtext, icon, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool StatNode::init(
        const std::string& text, const std::string& subtext,
        const std::string& icon, float width
    ) {
        if (!CCNode::init()) return false;

        setContentSize({ width, 40.0f });
        setAnchorPoint({ 0.5f, 0.5f });

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setContentSize(getContentSize() * 2.0f);
        bg->setScale(0.5f);
        addChildAtPosition(
            bg,
            Anchor::Center,
            {}
        );

        auto iconBG = NineSlice::create("square02b_001.png");
        iconBG->setColor({ 0, 0, 0 });
        iconBG->setOpacity(51);
        iconBG->setContentSize({ 50.0f, 50.0f });
        iconBG->setScale(0.5f);
        addChildAtPosition(
            iconBG,
            Anchor::Left,
            { getContentHeight() / 2.0f, 0.0f }
        );

        m_icon = CCSprite::create(icon.c_str());
        m_icon->setScale(16.0f / m_icon->getContentWidth());
        addChildAtPosition(
            m_icon,
            Anchor::Left,
            { getContentHeight() / 2.0f, 0.0f }
        );

        m_textLabel = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        m_textLabel->setScale(0.35f);
        m_textLabel->setAnchorPoint({ 0.0f, 1.0f });
        addChildAtPosition(
            m_textLabel,
            Anchor::Left,
            { (getContentHeight() - iconBG->getScaledContentHeight()) / 2.0f + iconBG->getScaledContentWidth() + 5.0f, 10.0f }
        );

        m_subtextLabel = CCLabelBMFont::create(subtext.c_str(), "chatFont.fnt");
        m_subtextLabel->setScale(0.5f);
        m_subtextLabel->setAnchorPoint({ 0.0f, 0.0f });
        addChildAtPosition(
            m_subtextLabel,
            Anchor::Left,
            { (getContentHeight() - iconBG->getScaledContentHeight()) / 2.0f + iconBG->getScaledContentWidth() + 5.0f, -10.0f }
        );

        return true;
    }
}