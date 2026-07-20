#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace Utils {
    // Hi, Ery =3
    inline void removePlacement(int levelID, CCNode* gdlLabel, CCNode* gdlIcon, const std::unordered_map<CCNode*, float> origPositions, const bool onLevelCell) {
        GDL::Cache::Levels::setLevelWOPlacement(levelID);
        if (gdlLabel) gdlLabel->setVisible(false);
        if (gdlIcon) gdlIcon->setVisible(false);
        
        for (auto& [node, pos] : origPositions) {
            if (node) {
                onLevelCell
                ? node->setPositionX(pos)
                : node->setPositionY(pos);
            }
        }
    }
};