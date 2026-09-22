#pragma once

namespace TailyUI {
    class LabeledCheckbox : public cocos2d::CCMenu {
    public:
        static LabeledCheckbox* create(
            const char* name, float height, const char* info,
            geode::Function<void(bool)> togglerCallback
        );
        
        void setToggled(bool enable);
        void setTogglerCallback(geode::Function<void(bool)> togglerCallback);

    protected:
        geode::Function<void(bool)> m_togglerCallback = nullptr;

        CCMenuItemToggler* m_toggler;
        cocos2d::CCLabelBMFont* m_label;
        CCMenuItemSpriteExtra* m_infoBtn;

        bool init(
            const char* name, float height, const char* info,
            geode::Function<void(bool)> togglerCallback
        );
    };
}