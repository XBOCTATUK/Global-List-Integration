#pragma once

namespace TailyUI {
    class PageSelector : public cocos2d::CCMenu {
    public:
        static PageSelector* create(
            int maxPage, geode::Function<void(int)> callback
        );
        void page(int page);

    protected:
        int m_page = 1;
        int m_maxPage = INT_MAX;

        geode::Function<void(int)> m_callback;

        CCMenuItemSpriteExtra* m_leftBtn;
        CCMenuItemSpriteExtra* m_rightBtn;
        cocos2d::CCLabelBMFont* m_pageLabel;

        bool init(
            int maxPage, geode::Function<void(int)> callback
        );
    };
}