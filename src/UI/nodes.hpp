#include "../Models/GDLUser.hpp"

namespace TailyUI {
    cocos2d::CCNode* createStatNode(
        const std::string& text,
        const std::string& subtext,
        const std::string& icon,
        float width
    );
    cocos2d::CCNode* createUserLevelsNode(
        const OptGDLBasicLevels& levels,
        const std::string& text,
        const std::string& icon,
        float width
    );
    cocos2d::CCNode* createLevelButtonSprite(const GDLBasicLevel& level);
}