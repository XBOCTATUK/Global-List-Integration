#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace Utils {
    inline void failure(const char* title, const char* text) {
        auto alertLayer = FLAlertLayer::create(title,  text, "OK");
        alertLayer->m_scene = CCDirector::get()->getRunningScene();
        alertLayer->show();
    }
};