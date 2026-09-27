#pragma once

#include "../../Models/GDLCountryUser.hpp"
#include "../../UI/PageSelector/PageSelector.hpp"

namespace TailyUI {
    class PlayerList : public cocos2d::CCNode {
    public:
        static PlayerList* create(
            const std::vector<GDLCountryUser>& users, float width
        );

    protected:
        int m_perPage = 50;

        std::vector<GDLCountryUser> m_users;

        geode::NineSlice* m_bg;
        cocos2d::CCSprite* m_icon;
        cocos2d::CCLabelBMFont* m_textLabel;
        cocos2d::CCLabelBMFont* m_countLabel;
        TailyUI::PageSelector* m_topPageMenu;
        TailyUI::PageSelector* m_bottomPageMenu;
        cocos2d::CCNode* m_listNode;

        bool init(
            const std::vector<GDLCountryUser>& users, float width
        );
        void page(int page);

        cocos2d::CCNode* createPlayerListCell(
            const GDLCountryUser& user, int placement, float width
        );
        void updateUI();
    };
}