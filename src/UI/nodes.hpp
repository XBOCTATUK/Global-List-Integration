#include "../Models/GDLUser.hpp"
#include "../Models/GDLCountryUser.hpp"

namespace TailyUI {
    cocos2d::CCNode* createStatNode(
        const std::string& text, const std::string& subtext,
        const std::string& icon, float width
    );
    cocos2d::CCNode* createUserLevelsNode(
        const OptGDLBasicLevels& levels, const std::string& text,
        const std::string& icon, float width
    );
    cocos2d::CCNode* createPlayerListNode(const std::vector<GDLCountryUser>& users, float width);

    cocos2d::CCNode* createLevelButtonSprite(const GDLBasicLevel& level);
    cocos2d::CCNode* createPlayerListCell(const GDLCountryUser& user, int placement, float width);
}