#pragma once

#include <initializer_list>
#include <unordered_map>

namespace TailyUI {
    class OptionBar : public cocos2d::CCNode {
    public:
        static OptionBar* create(
            const char* icon, float labelScale, 
            const std::initializer_list<std::string>& values,
            geode::Function<void(const std::string&, bool)> choiceCallback,
            geode::Function<void()> optionsCallback
        );

        void activateChoice(const std::string& choiceName);
        void setChoiceCallback(geode::Function<void(const std::string&, bool)> callback);
        void setOptionsCallback(geode::Function<void()> callback);

    protected:
        cocos2d::CCSprite* m_icon = nullptr;
        cocos2d::CCMenu* m_menu = nullptr;

        std::unordered_map<std::string, CCMenuItemSpriteExtra*> m_choices;
        CCMenuItemSpriteExtra* m_selectedBtn = nullptr;

        geode::Function<void(const std::string&, bool)> m_choiceCallback = nullptr;
        geode::Function<void()> m_optionsCallback = nullptr;

        bool init(
            const char* icon, float labelScale, 
            const std::initializer_list<std::string>& values,
            geode::Function<void(const std::string&, bool)> choiceCallback,
            geode::Function<void()> optionsCallback
        );
    };
}