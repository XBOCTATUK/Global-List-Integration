#pragma once

#include <Geode/loader/Mod.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

namespace Settings {
    bool shouldLoadPlacement();
    void setShouldLoadPlacement(bool enable);

    bool increaseLevelsPerPage();
};