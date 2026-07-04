#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace Utils {
    inline void failure(const char* title, const char* whatsFailed) {
        auto alertLayer = FLAlertLayer::create(title,  whatsFailed, "OK");
        alertLayer->m_scene = CCDirector::get()->getRunningScene();
        alertLayer->show();
    }
};