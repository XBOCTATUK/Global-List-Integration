#include "PageSelector.hpp"

using namespace geode::prelude;

namespace TailyUI {
    PageSelector* PageSelector::create(
        int maxPage, geode::Function<void(int)> callback
    ) {
    	auto ret = new PageSelector();
        if (ret && ret->init(maxPage, std::move(callback))) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool PageSelector::init(
        int maxPage, geode::Function<void(int)> callback
    ) {
        if (!CCMenu::init()) return false;

        setContentSize({ 80.0f, 20.0f });
        setAnchorPoint({ 0.5f, 0.5f });

        m_maxPage = maxPage;
        m_callback = std::move(callback);

        auto leftArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        leftArrow->setScale(16.0f / leftArrow->getContentHeight());

        m_leftBtn = CCMenuItemExt::createSpriteExtra(
            leftArrow, [this](CCMenuItemSpriteExtra* self) {
                page(m_page - 1);

                if (m_callback) {
                    m_callback(m_page);
                }
            }
        );
        m_leftBtn->setVisible(false);
        addChildAtPosition(
            m_leftBtn,
            Anchor::Left,
            { m_leftBtn->getContentWidth() / 2.0f, 0.0f }
        );

        auto rightArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        rightArrow->setScale(16.0f / rightArrow->getContentHeight());
        rightArrow->setFlipX(true);

        m_rightBtn = CCMenuItemExt::createSpriteExtra(
            rightArrow, [this](auto) {
                page(m_page + 1);

                if (m_callback) {
                    m_callback(m_page);
                }
            }
        );
        addChildAtPosition(
            m_rightBtn,
            Anchor::Right,
            { -m_rightBtn->getContentWidth() / 2.0f, 0.0f }
        );

        m_pageLabel = CCLabelBMFont::create(
            fmt::format("{}/{}", m_page, m_maxPage).c_str(),
            "bigFont.fnt"
        );
        m_pageLabel->setScale(0.4f);
        addChildAtPosition(
            m_pageLabel,
            Anchor::Center,
            {}
        );

        return true;
    }

    void PageSelector::page(int page) {
        m_page = std::clamp(page, 1, m_maxPage);

        m_leftBtn->setVisible(m_page > 1);
        m_rightBtn->setVisible(m_page < m_maxPage);
        m_pageLabel->setString(
            fmt::format("{}/{}", m_page, m_maxPage).c_str()
        );
    }
}