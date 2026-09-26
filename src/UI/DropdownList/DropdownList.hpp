#pragma once

namespace TailyUI {
    enum class State { Closed, Opening, Opened, Closing };
    enum class Region { None, Header, Scroll };

    class DropdownList : public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate {
    public:
        using SelectionCallback = geode::Function<void(std::size_t, const std::string&)>;

        static DropdownList* create(
            const std::vector<std::string>& values,
            float width,
            size_t maxVisibleRows = 4,
            SelectionCallback callback = {}
        );

        const std::vector<std::string>& getValues() const;
        const std::string& getSelectedValue() const;
        size_t getSelectedIndex() const;

        void setContentWidth(float width);
        void setValues(const std::vector<std::string>& values);
        void setSelectedIndex(size_t index, bool call = false);
        void setSelectionCallback(SelectionCallback callback);

        State getState() const;
        void setOpen(bool open);

    protected:
        SelectionCallback m_callback = nullptr;

        std::vector<std::string> m_values;
        size_t m_selectedIndex = SIZE_MAX;
        size_t m_maxVisibleRows = 4;
        float m_maxScrollHeight = 0.0f;
        
        geode::NineSlice* m_bg = nullptr;
        geode::NineSlice* m_border = nullptr;
        cocos2d::CCSprite* m_arrowSpr = nullptr;
        cocos2d::CCLabelBMFont* m_valueLabel = nullptr;
        
        cocos2d::CCClippingNode* m_clippingNode = nullptr;
        geode::ScrollLayer* m_scrollLayer = nullptr;
        std::vector<cocos2d::CCLayerColor*> m_valueBGs;
        
        State m_state = State::Closed;
        float m_elapsed = 0.0f;

        cocos2d::CCPoint m_touchStart{};
        Region m_touchRegion = Region::None;
        bool m_touchMoved = false;

        bool init(
            const std::vector<std::string>& values,
            float width,
            size_t maxVisibleRows,
            SelectionCallback callback
        );
        void addValue(const std::string& value);
        void updateHeight(float dt);

        void onClick(CCObject*);
        virtual void onEnter() override;
        virtual void onExit() override;

        virtual void update(float dt) override;
        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    };
}