#pragma once

#include <Geode/Geode.hpp>
#include <initializer_list>
#include <unordered_map>

using namespace geode::prelude;

namespace TailyUI {
    class OptionBar : public CCNode {
    public:
        static OptionBar* create(
            const char* icon, float labelScale, 
            const std::initializer_list<std::string>& values,
            Function<void(std::string, bool)> choiceCallback,
            Function<void()> optionsCallback
        );

        void activateChoice(std::string choiceName);
        void setChoiceCallback(Function<void(std::string, bool)> callback);
        void setOptionsCallback(Function<void()> callback);

    protected:
        CCSprite* m_icon;
        CCMenu* m_menu;

        std::unordered_map<std::string, CCMenuItemSpriteExtra*> m_choices;
        CCMenuItemSpriteExtra* m_selectedBtn;

        Function<void(std::string, bool)> m_choiceCallback = nullptr;
        Function<void()> m_optionsCallback = nullptr;

        bool init(
            const char* icon, float labelScale, 
            const std::initializer_list<std::string>& values,
            Function<void(std::string, bool)> choiceCallback,
            Function<void()> optionsCallback
        );
    };
};