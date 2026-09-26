#pragma once

#include "../../Models/GDLUser.hpp"
#include "../../Models/GDLCountry.hpp"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"

class GDLLeaderboardCell : public cocos2d::CCNode {
public:
    static GDLLeaderboardCell* create(const GDLUser& userData);
    static GDLLeaderboardCell* create(const GDLCountry& countryData, CountriesLeaderboardType type);

protected:
    bool init(const GDLUser& userData);
    bool init(const GDLCountry& countryData, CountriesLeaderboardType type);

    GDLUser m_userData;
    GDLCountry m_countryData;
    CountriesLeaderboardType m_type;

    cocos2d::CCLabelBMFont* m_placementLabel;
    CCMenuItemSpriteExtra* m_nameBtn;
    CCMenuItemSpriteExtra* m_badgeBtn;
    cocos2d::CCSprite* m_flagSpr;
    CCMenuItemSpriteExtra* m_flagBtn;
    cocos2d::CCLabelBMFont* m_pointsLabel;
};