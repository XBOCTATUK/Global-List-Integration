#include <Geode/modify/LevelSearchLayer.hpp>
#include "../layers/GlobalListLayer/GlobalListLayer.hpp"

class $modify(MyLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;

        auto filterMenu = this->getChildByID("other-filter-menu");

        auto globalListSpr = CircleButtonSprite::createWithSprite("global-list.png"_spr);
        globalListSpr->getTopNode()->setScale(1.1f);
        globalListSpr->setScale(0.8f);
        auto globalListBtn = CCMenuItemSpriteExtra::create(globalListSpr, this, menu_selector(MyLevelSearchLayer::onBtn));
        globalListBtn->setID("global-list-button");
        filterMenu->addChild(globalListBtn);
        filterMenu->updateLayout();

        return true;
    }

    void onBtn(CCObject* sender) {
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, GlobalListLayer::scene()));
    }
};