#pragma once

#include "../../Models/GDLUser.hpp"

namespace TailyUI {
    class UserLevelsNode : public cocos2d::CCNode {
    public:
        static UserLevelsNode* create(
            const OptGDLBasicLevels& levels, const std::string& text,
            const std::string& icon, float width
        );

    protected:
        cocos2d::CCSprite* m_icon;
        cocos2d::CCLabelBMFont* m_textLabel;
        cocos2d::CCLabelBMFont* m_countLabel;
        cocos2d::CCMenu* m_levelsMenu;

        bool init(
            const OptGDLBasicLevels& levels, const std::string& text,
            const std::string& icon, float width
        );
        cocos2d::CCNode* createLevelButtonSprite(const GDLBasicLevel& level);
    };
}