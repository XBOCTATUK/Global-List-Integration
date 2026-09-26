#pragma once

#include "../../Models/GDLUser.hpp"
#include "../../Models/GDLCountry.hpp"

class GDLLeaderboardCell : public cocos2d::CCNode {
public:
    static GDLLeaderboardCell* create(const GDLUser& userData);
    static GDLLeaderboardCell* create(const GDLCountry& countryData, CountriesLeaderboardType type);

protected:
    bool init(const GDLUser& userData);
    bool init(const GDLCountry& countryData, CountriesLeaderboardType type);

    GDLUser m_userData{};
    GDLCountry m_countryData{};
    CountriesLeaderboardType m_type{};

    cocos2d::CCLabelBMFont* m_placementLabel = nullptr;
    CCMenuItemSpriteExtra* m_nameBtn = nullptr;
    CCMenuItemSpriteExtra* m_badgeBtn = nullptr;
    cocos2d::CCSprite* m_flagSpr = nullptr;
    CCMenuItemSpriteExtra* m_flagBtn = nullptr;
    cocos2d::CCLabelBMFont* m_pointsLabel = nullptr;
};