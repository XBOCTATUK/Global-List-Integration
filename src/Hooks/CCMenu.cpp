#include <Geode/modify/CCMenu.hpp>

using namespace geode::prelude;

class $modify(CCMenu) {
    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        bool handled = CCMenu::ccTouchBegan(touch, event);

        auto userObj = getUserObject("gdl-block-touches");
        if (!userObj) return handled;

        auto isBlockTouchesEnabled = typeinfo_cast<CCBool*>(userObj);
        if (!isBlockTouchesEnabled || !isBlockTouchesEnabled->getValue()) return handled;

        auto point = convertTouchToNodeSpace(touch);
        bool inside = CCRect{{0.0f, 0.0f}, getContentSize()}.containsPoint(point);

        return inside || handled;
    }
};