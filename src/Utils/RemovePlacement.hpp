#pragma once

#include "../Cache/Levels/Levels.hpp"

namespace Utils {
    // Hi, Ery =3
    inline void removePlacement(
        int levelID, cocos2d::CCNode* gdlLabel, cocos2d::CCNode* gdlIcon,
        const std::unordered_map<cocos2d::CCNode*, float> origPositions, const bool onLevelCell
    ) {
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
}