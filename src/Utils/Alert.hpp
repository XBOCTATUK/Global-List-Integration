#pragma once

namespace Utils {
    inline void alert(const char* title, const std::string& text) {
        auto alertLayer = FLAlertLayer::create(title,  text, "OK");
        alertLayer->m_scene = cocos2d::CCDirector::get()->getRunningScene();
        alertLayer->show();
    }
}