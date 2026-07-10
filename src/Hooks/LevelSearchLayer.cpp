#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include "../Layers/GDLListLayer/GDLListLayer.hpp"

using namespace geode::prelude;

class $modify(MyLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;

        auto filterMenu = getChildByID("other-filter-menu");
        if (!filterMenu) return true;

        auto spr = CircleButtonSprite::createWithSprite("global-list.png"_spr, 0.9f, CircleBaseColor::Green, CircleBaseSize::Small);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyLevelSearchLayer::onBtn));
        btn->setID("global-list-button"_spr);
        
        filterMenu->addChild(btn);
        filterMenu->updateLayout();

        return true;
    }

    void onBtn(CCObject* sender) {
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, GDLListLayer::scene()));
    }
};