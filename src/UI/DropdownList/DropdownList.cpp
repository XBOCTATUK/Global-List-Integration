#include "DropdownList.hpp"
#include "../SimpleClippingNode/SimpleClippingNode.hpp"
#include "../../Utils/LimitLabelWithDots.hpp"
#include <cstdint>

using namespace geode::prelude;

constexpr float animDuration = 0.4f;

namespace TailyUI {
    DropdownList* DropdownList::create(
        const std::vector<std::string>& values,
        float width,
        size_t maxVisibleRows,
        SelectionCallback callback
    ) {
    	auto ret = new DropdownList();
        if (ret && ret->init(values, width, maxVisibleRows, std::move(callback))) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool DropdownList::init(
        const std::vector<std::string>& values,
        float width,
        size_t maxVisibleRows,
        SelectionCallback callback
    ) {
        if (!CCNode::create()) return false;
        if (values.empty() || width < 50.0f) return false;

        setContentSize({ width, 30.0f });
        setAnchorPoint({ 0.5f, 0.5f });
        

        m_values.reserve(values.size());
        m_maxVisibleRows = maxVisibleRows;
        m_maxScrollHeight = std::min(30.0f * m_maxVisibleRows + 15.0f, 30.0f * values.size());
        m_callback = std::move(callback);

        m_bg = NineSlice::create("square02b_001.png");
        m_bg->setColor({126, 74, 40 });
        m_bg->setAnchorPoint({ 0.0f, 1.0f });
        m_bg->setPosition({ 0.0f, getContentHeight() });
        m_bg->setContentSize(getContentSize() * 2.0f);
        m_bg->setScale(0.5f);
        addChild(m_bg);

        m_border = NineSlice::create("border.png"_spr);
        m_border->setColor(m_bg->getColor());
        m_border->setAnchorPoint(m_bg->getAnchorPoint());
        m_border->setPosition(m_bg->getPosition());
        m_border->setContentSize(m_bg->getContentSize());
        m_border->setScale(m_bg->getScale());
        addChild(m_border, 1);

        m_arrowSpr = CCSprite::createWithSpriteFrameName("d_arrow_03_001.png");
        m_arrowSpr->setRotation(90.0f);
        m_arrowSpr->setScaleX(0.6f);
        m_arrowSpr->setScaleY(0.4f);
        m_arrowSpr->setPosition({ getContentWidth() - getContentHeight() / 2.0f, getContentHeight() / 2.0f });
        addChild(m_arrowSpr, 2);

        m_valueLabel = CCLabelBMFont::create("", "bigFont.fnt");
        m_valueLabel->setScale(0.5f);
        Utils::limitLabelWithDots(
            m_valueLabel,
            m_arrowSpr->getPositionX() - m_arrowSpr->getScaledContentHeight() / 2.0f - 10.0f
        );
        m_valueLabel->setAnchorPoint({ 0.0f, 0.5f });
        m_valueLabel->setPosition({ 7.0f, getContentHeight() / 2.0f });
        addChild(m_valueLabel, 2);

        m_clippingNode = TailyUI::SimpleClippingNode::create({ width, 0.0f });
        m_clippingNode->setAnchorPoint({ 0.0f, 1.0f });
        m_clippingNode->setPosition({ 0.0f, 0.0f });
        m_clippingNode->setVisible(false);
        addChild(m_clippingNode);

        m_scrollLayer = ScrollLayer::create({width - 4.0f, m_maxScrollHeight});
        m_scrollLayer->setAnchorPoint({ 0.0f, 0.0f });
        m_scrollLayer->setPosition({ 2.0f, 0.0f });
        m_scrollLayer->ignoreAnchorPointForPosition(false);
        m_scrollLayer->m_peekLimitTop = 15.0f;
        m_scrollLayer->m_peekLimitBottom = 15.0f;
        m_scrollLayer->setMouseEnabled(false);
        m_scrollLayer->setTouchPriority(-256);
        m_clippingNode->addChild(m_scrollLayer);

        auto content = m_scrollLayer->m_contentLayer;
        content->setLayout(ScrollLayer::createDefaultListLayout(0.0f));

        setValues(values);
        setSelectedIndex(0);
        
        return true;
    }

    const std::vector<std::string>& DropdownList::getValues() const {
        return m_values;
    }

    const std::string& DropdownList::getSelectedValue() const {
        return m_values[m_selectedIndex];
    }

    size_t DropdownList::getSelectedIndex() const {
        return m_selectedIndex;
    }

    void DropdownList::setContentWidth(float width) {
        if (width < 50.0f) return;
        CCNode::setContentWidth(width);

        m_bg->setContentSize({ width * 2.0f, m_bg->getContentHeight() });
        m_border->setContentSize(m_bg->getContentSize());
        m_arrowSpr->setPosition({ width - getContentHeight() / 2.0f, getContentHeight() / 2.0f });

        Utils::limitLabelWithDots(
            m_valueLabel,
            m_arrowSpr->getPositionX() - m_arrowSpr->getScaledContentHeight() / 2.0f - 10.0f
        );

        m_clippingNode->setContentSize({ width, 0.0f });
        m_scrollLayer->setContentSize({ width - 4.0f, m_maxScrollHeight });
        m_scrollLayer->m_contentLayer->setContentWidth(width - 4.0f);

        setValues(m_values);
    }

    void DropdownList::setValues(const std::vector<std::string>& values) {
        auto content = m_scrollLayer->m_contentLayer;
        content->removeAllChildrenWithCleanup(true);
        m_selectedIndex = SIZE_MAX;
        m_valueBGs.clear();

        m_values = values;
        m_valueBGs.reserve(m_values.size());
        for (const auto& value : m_values) {
            addValue(value);
        }
        content->updateLayout();

        setSelectedIndex(0);
        
        m_maxScrollHeight = std::min(30.0f * m_maxVisibleRows + 15.0f, 30.0f * values.size());
        m_scrollLayer->setContentHeight(m_maxScrollHeight);
        m_scrollLayer->scrollToTop();
    }

    void DropdownList::setSelectedIndex(size_t index, bool call) {
        if (index >= m_values.size()) return;

        if (m_selectedIndex != SIZE_MAX) {
            bool isParity = m_selectedIndex % 2 == 0;
            auto bgColor = isParity ?  ccColor3B{161, 88, 44} : ccColor3B{194, 114, 62};

            auto prevValueBG = m_valueBGs[m_selectedIndex];
            if (prevValueBG) {
                prevValueBG->setColor(bgColor);
            }
        }

        m_selectedIndex = index;

        m_valueLabel->setString(getSelectedValue().c_str());
        Utils::limitLabelWithDots(
            m_valueLabel,
            m_arrowSpr->getPositionX() - m_arrowSpr->getScaledContentHeight() / 2.0f - 10.0f
        );

        auto valueBG = m_valueBGs[m_selectedIndex];
        if (valueBG) {
            valueBG->setColor({105, 150, 75});
        }

        if (m_callback && call) {
            m_callback(getSelectedIndex(), getSelectedValue());
        }
    }

    void DropdownList::setSelectionCallback(SelectionCallback callback) {
        m_callback = std::move(callback);
    }

    State DropdownList::getState() const {
        return m_state;
    }

    void DropdownList::setOpen(bool open) {
        if ((open && m_state == State::Opened) || (!open && m_state == State::Closed)) return;

        if (open) m_state = State::Opening;
        else m_state = State::Closing;

        float deltaAngle = open ? -180.0f : 180.0f;
        auto rotateAnim = CCRotateBy::create(animDuration, deltaAngle);

        auto easeRotateAnim = CCEaseExponentialOut::create(rotateAnim);
        m_arrowSpr->runAction(easeRotateAnim);

        if (open) {
            // m_scrollLayer->setMouseEnabled(true);
            m_clippingNode->setVisible(true);
        }

        unschedule(schedule_selector(DropdownList::updateHeight));

        m_elapsed = 0.0f;
        schedule(schedule_selector(DropdownList::updateHeight));
    }
    
    void DropdownList::addValue(const std::string& value) {
        auto node = CCNode::create();
        node->setContentSize({ m_scrollLayer->getContentWidth(), 30.0f });

        auto content = m_scrollLayer->m_contentLayer;

        bool isParity = content->getChildrenCount() % 2 == 0;
        auto bgColor = isParity ? ccColor4B{161, 88, 44, 255} : ccColor4B{194, 114, 62, 255};

        auto bg = CCLayerColor::create(bgColor, m_scrollLayer->getContentWidth(), 30.0f);
        bg->setAnchorPoint({ 0.0f, 0.0f });
        bg->setPosition({ 0.0f, 0.0f });

        m_valueBGs.push_back(bg);
        node->addChild(bg);

        auto label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
        label->setScale(0.5f);
        label->setAnchorPoint({ 0.0f, 0.5f });
        label->setPosition({ 5.0f, node->getContentHeight() / 2.0f });
        Utils::limitLabelWithDots(label, node->getContentWidth() - 10.0f);
        node->addChild(label, 2);

        content->addChild(node);
    }

    void DropdownList::updateHeight(float dt) {
        m_elapsed += dt;
        float t = std::min(m_elapsed / animDuration, 1.0f);

        t = t >= 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
        float maxBGHeight = 30.0f + m_maxScrollHeight + 2.0f;

        auto startScrollHeight = m_state == State::Opening ? -2.0f : m_maxScrollHeight;
        auto targetScrollHeight = m_state == State::Opening ? m_maxScrollHeight : -2.0f;
        auto startBGHeight = (m_state == State::Opening ? 30.0f : maxBGHeight) * 2.0f;
        auto targetBGHeight = (m_state == State::Opening ? maxBGHeight : 30.0f) * 2.0f;
    
        m_bg->setContentHeight(std::lerp(startBGHeight, targetBGHeight, t));
        m_border->setContentHeight(std::lerp(startBGHeight, targetBGHeight, t));
        m_clippingNode->setContentHeight(std::lerp(startScrollHeight, targetScrollHeight, t));

        if (t >= 1.0f) {
            m_state = m_state == State::Opening ? State::Opened : State::Closed;

            if (m_state == State::Closed) {
                m_clippingNode->setVisible(false);
            }

            unschedule(schedule_selector(DropdownList::updateHeight));
            return;
        }
    }

    void DropdownList::onClick(CCObject*) {
        setOpen(m_state == State::Closed);
    }

    void DropdownList::onEnter() {
        CCNode::onEnter();

        CCDirector::get()->getTouchDispatcher()->addTargetedDelegate(this, -256, true);
        scheduleUpdate();
    }

    void DropdownList::onExit() {
        CCDirector::get()->getTouchDispatcher()->removeDelegate(this);
        unscheduleUpdate();

        CCNode::onExit();
    }

    void DropdownList::update(float dt) {
        CCNode::update(dt);

        auto mousePos = getMousePos();
        auto mousePosOnNode = convertToNodeSpace(mousePos);
        bool insideScroll = CCRect{{0.0f, -m_clippingNode->getContentHeight() - 2.0f}, m_clippingNode->getContentSize()}.containsPoint(mousePosOnNode);

        m_scrollLayer->setMouseEnabled(insideScroll && m_state != State::Closed);
    }

    bool DropdownList::ccTouchBegan(CCTouch* touch, CCEvent* event) {
        auto point = convertTouchToNodeSpace(touch);
        bool insideHeader = CCRect{{0.0f, 0.0f}, getContentSize()}.containsPoint(point);
        bool insideScroll = CCRect{{0.0f, -m_clippingNode->getContentHeight() - 2.0f}, m_clippingNode->getContentSize()}.containsPoint(point);

        m_touchStart = point;
        if (insideHeader) {
            m_touchRegion = Region::Header;
        }
        else if (insideScroll) {
            m_touchRegion = Region::Scroll;
        }

        if (insideHeader || insideScroll) return true;

        return false;
    }

    void DropdownList::ccTouchMoved(CCTouch* touch, CCEvent* event) {
        bool isAnimating = m_state == State::Opening || m_state == State::Closing;
        if (isAnimating) return;

        auto point = convertTouchToNodeSpace(touch);

        if (m_touchStart.getDistance(point) > 3.0f) {
            m_touchMoved = true;
        }
    }

    void DropdownList::ccTouchEnded(CCTouch* touch, CCEvent* event) {
        bool isAnimating = m_state == State::Opening || m_state == State::Closing;
        if (isAnimating) return;

        CCPoint point = convertTouchToNodeSpace(touch);
        bool insideHeader = CCRect{{0.0f, 0.0f}, getContentSize()}.containsPoint(point);
        bool insideScroll = CCRect{{0.0f, -m_clippingNode->getContentHeight() - 2.0f}, m_clippingNode->getContentSize()}.containsPoint(point);

        if (insideHeader && m_touchRegion == Region::Header && !m_touchMoved) {
            onClick(nullptr);
        }
        else if (insideScroll && m_touchRegion == Region::Scroll && !m_touchMoved) {
            float contentHeight = m_scrollLayer->m_contentLayer->getContentHeight();
            float scrollHeight = m_scrollLayer->getContentHeight();
            float contentPosY = m_scrollLayer->m_contentLayer->getPositionY();

            float touchPointY = contentHeight - scrollHeight + contentPosY - point.y;
            if (touchPointY < 0.0f || touchPointY > contentHeight) return;

            setSelectedIndex(
                std::clamp(
                    static_cast<size_t>(touchPointY / 30.0f),
                    static_cast<size_t>(0),
                    m_values.size()-1
                ),
                true
            );
        }

        m_touchMoved = false;
    }

    void DropdownList::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
        m_touchMoved = false;
    }
}