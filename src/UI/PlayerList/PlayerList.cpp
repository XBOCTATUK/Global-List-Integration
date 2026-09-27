#include "PlayerList.hpp"
#include "../../Popups/UserInfoPopup/UserInfoPopup.hpp"
#include "../../Events/UpdateScrollEvent.hpp"

using namespace geode::prelude;

namespace TailyUI {
    PlayerList* PlayerList::create(
        const std::vector<GDLCountryUser>& users, float width
    ) {
    	auto ret = new PlayerList();
        if (ret && ret->init(users, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool PlayerList::init(
        const std::vector<GDLCountryUser>& users, float width
    ) {
        if (!CCNode::init()) return false;

        setContentWidth(width);
        setAnchorPoint({ 0.5f, 0.5f });
        m_users = users;

        m_bg = NineSlice::create("square02b_001.png");
        m_bg->setColor({ 0, 0, 0 });
        m_bg->setOpacity(51);
        m_bg->setScale(0.5f);
        m_bg->setID("background");

        m_icon = CCSprite::create("blue-players-icon.png"_spr);
        m_icon->setScale(12.0f / m_icon->getContentWidth());
        m_icon->setID("icon");

        m_textLabel = CCLabelBMFont::create("Players", "bigFont.fnt");
        m_textLabel->setScale(0.35f);
        m_textLabel->setAnchorPoint({ 0.0f, 0.5f });
        m_textLabel->setID("title-text");

        m_countLabel = CCLabelBMFont::create(
            fmt::format("{}", users.size()).c_str(),
            "bigFont.fnt"
        );
        m_countLabel->setScale(0.35f);
        m_countLabel->setAnchorPoint({ 1.0f, 0.5f });
        m_countLabel->setID("count-text");

        m_topPageMenu = TailyUI::PageSelector::create(
            users.size() / m_perPage + 1, [this](int currentPage) {
                page(currentPage);

                m_bottomPageMenu->page(currentPage);
            }
        );
        m_topPageMenu->setID("top-page-menu");

        m_bottomPageMenu = TailyUI::PageSelector::create(
            users.size() / m_perPage + 1, [this](int currentPage) {
                page(currentPage);

                m_topPageMenu->page(currentPage);
            }
        );
        m_bottomPageMenu->setID("bottom-page-menu");

        m_listNode = CCNode::create();
        m_listNode->setContentSize({ getContentWidth(), 0.0f });
        m_listNode->setAnchorPoint({ 0.5f, 1.0f });
        m_listNode->setLayout(
            SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Center)
            ->setMainAxisScaling(AxisScaling::Fit)
            ->setGap(2.0f)
            ->ignoreInvisibleChildren(true)
            ->setPadding({ 7.5f, 0, 7.5f, 0.0f })
        );
        m_listNode->setID("list-node");

        page(1);

        addChildAtPosition(
            m_bg,
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
            m_topPageMenu,
            Anchor::Top,
            { 0.0f, -10.0f }
        );
        addChildAtPosition(
            m_bottomPageMenu,
            Anchor::Bottom,
            { 0.0f, 10.0f }
        );
        addChildAtPosition(
            m_listNode,
            Anchor::Top,
            { 0.0f, -20.0f }
        );

        return true;
    }

    void PlayerList::page(int page) {
        m_listNode->removeAllChildren();
        size_t begin = m_perPage * (page-1);
        size_t end = std::min(
            static_cast<size_t>(m_perPage * page), m_users.size()
        );

        for (int i = begin; i < end; i++) {
            auto user = m_users.at(i);

            auto playerListCell = createPlayerListCell(
                user, i+1, getContentWidth() - 15.0f
            );
            m_listNode->addChild(playerListCell);
        }
        m_listNode->updateLayout();

        updateUI();
    }

    CCNode* PlayerList::createPlayerListCell(
        const GDLCountryUser& user, int placement, float width
    ) {
        auto node = CCNode::create();
        node->setContentSize({ width, 30.0f });
        node->setAnchorPoint({ 0.5f, 0.5f });

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(51);
        bg->setContentSize(node->getContentSize() * 2.0f);
        bg->setScale(0.5f);
        bg->setID("background");
        node->addChildAtPosition(
            bg,
            Anchor::Center,
            {}
        );

        auto menu = CCMenu::create();
        menu->setContentSize(node->getContentSize());
        menu->setAnchorPoint({ 0.5f, 0.5f });
        menu->ignoreAnchorPointForPosition(false);
        menu->setID("main-menu");
        node->addChildAtPosition(
            menu,
            Anchor::Center,
            {}
        );

        auto usernameLabel = CCLabelBMFont::create(
            fmt::format("{}. {}", placement, user.username).c_str(), "bigFont.fnt"
        );
        usernameLabel->setScale(0.5f);

        auto usernameBtn = CCMenuItemExt::createSpriteExtra(
            usernameLabel, [user](auto) {
                UserInfoPopup::create(user.id)->show();
            }
        );
        usernameBtn->setID("username-button");
        menu->addChildAtPosition(
            usernameBtn,
            Anchor::Left,
            { 10.0f + usernameBtn->getContentWidth() / 2.0f, 0.0f }
        );

        auto pointsLabel = CCLabelBMFont::create(
            fmt::format("{:.2f}", user.points).c_str(), "bigFont.fnt"
        );
        pointsLabel->setScale(0.5f);
        pointsLabel->setColor({ 0, 212, 255 });
        pointsLabel->setAnchorPoint({ 0.5f, 0.5f });
        pointsLabel->setID("points-text");
        node->addChildAtPosition(
            pointsLabel,
            Anchor::Right,
            { -pointsLabel->getScaledContentWidth() / 2.0f - 10.0f, 0.0f }
        );

        return node;
    }

    void PlayerList::updateUI() {
        setContentHeight(m_listNode->getContentHeight() + 40.0f);
        m_bg->setContentSize(getContentSize() * 2.0f);

        m_bg->setPosition({ getContentSize() / 2.0f });
        m_icon->setPosition({ 7.5f + m_icon->getScaledContentWidth() / 2.0f, getContentHeight() - 10.0f });
        m_textLabel->setPosition({ m_icon->getPositionX() + m_icon->getScaledContentWidth() / 2.0f + 3.0f, getContentHeight() - 10.0f });
        m_countLabel->setPosition(getContentSize() - ccp( 7.5f, 10.0f ));
        m_topPageMenu->setPosition({ getContentWidth() / 2.0f, getContentHeight() - 10.0f });
        m_bottomPageMenu->setPosition({ getContentWidth() / 2.0f, 10.0f });
        m_listNode->setPosition({ getContentWidth() / 2.0f, getContentHeight() - 20.0f });

        UpdateScrollEvent().send();
    }
}