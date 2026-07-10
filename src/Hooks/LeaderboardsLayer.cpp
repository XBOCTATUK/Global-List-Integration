#include <Geode/Geode.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
// #include "../Layers/GlobalListLeaderboards/GlobalListLeaderboards.hpp"

using namespace geode::prelude;

class $modify(MyLeaderboardsLayer, LeaderboardsLayer) {
    bool init(LeaderboardType type, LeaderboardStat stat) {
        if (!LeaderboardsLayer::init(type, stat)) return false;
        
        auto bottomRightMenu = getChildByID("bottom-right-menu");
        if (!bottomRightMenu) return true;

        auto spr = CircleButtonSprite::createWithSprite("global-list.png"_spr, 0.9f, CircleBaseColor::Green, CircleBaseSize::Small);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyLeaderboardsLayer::onBtn));
        btn->setID("global-list-button");

        bottomRightMenu->addChild(btn);
        bottomRightMenu->updateLayout();

        return true;
    }

    void onBtn(CCObject* sender) {
        // CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, GlobalListLeaderboards::scene()));
    }
};