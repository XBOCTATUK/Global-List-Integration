#pragma once

#include "../../UI/DropdownList/DropdownList.hpp"
#include "Geode/c++stl/string.hpp"

namespace TailyUI {
    enum class SearchBarType {
        WithFilters, WithCountrySelection, WithLeaderboardTypeSelection
    };

    class SearchBar : public cocos2d::CCNode {
    public:
        static SearchBar* create(geode::ZStringView placeholder, SearchBarType type, geode::Function<void(gd::string)> searchCallback);
        
        void setSearchBarType(SearchBarType type);
        void setSearchCallback(geode::Function<void(gd::string)> callback);
        
        geode::TextInput* getSearchInput();
        TailyUI::DropdownList* getDropdownList();

    protected:
        cocos2d::CCMenu* m_searchBarMenu = nullptr;
        geode::TextInput* m_searchInput = nullptr;
        CCMenuItemSpriteExtra* m_filterBtn = nullptr;
        TailyUI::DropdownList* m_dropdown = nullptr;
        CCMenuItemSpriteExtra* m_searchBtn = nullptr;

        geode::Function<void(gd::string)> m_searchCallback = nullptr;
        geode::Function<void(size_t, std::string)> m_dropdownCallback = nullptr;

        bool init(geode::ZStringView placeholder, SearchBarType type, geode::Function<void(gd::string)> searchCallback);
        void draw() override;
    };
}