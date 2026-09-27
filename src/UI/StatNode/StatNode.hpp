#pragma once

namespace TailyUI {
    class StatNode : public cocos2d::CCNode {
    public:
        static StatNode* create(
            const std::string& text, const std::string& subtext,
            const std::string& icon, float width
        );

    protected:
        cocos2d::CCSprite* m_icon;
        cocos2d::CCLabelBMFont* m_statLabel;
        cocos2d::CCLabelBMFont* m_statNameLabel;

        bool init(
            const std::string& text, const std::string& subtext,
            const std::string& icon, float width
        );
    };
}