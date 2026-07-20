#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace TailyUI {
    class LabeledCheckbox : public CCMenu {
    public:
        static LabeledCheckbox* create(
            const char* name, float height, const char* info,
            Function<void(bool)> togglerCallback
        );
        
        void setToggled(bool enable);
        void setTogglerCallback(Function<void(bool)> togglerCallback);

    protected:
        Function<void(bool)> m_togglerCallback = nullptr;

        CCMenuItemToggler* m_toggler;
        CCLabelBMFont* m_label;
        CCMenuItemSpriteExtra* m_infoBtn;

        bool init(
            const char* name, float height, const char* info,
            Function<void(bool)> togglerCallback
        );
    };
};