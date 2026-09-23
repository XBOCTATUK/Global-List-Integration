#include "../../Models/GDLUser.hpp"

class UserInfoPopup : public geode::Popup {
public:
	static UserInfoPopup* create(const GDLUser& userData);

protected:
	cocos2d::CCMenu* m_basicInfoMenu;
	cocos2d::CCLabelBMFont* m_usernameLabel;
	CCMenuItemSpriteExtra* m_badgeBtn;
    CCMenuItemSpriteExtra* m_flagBtn;

	

	bool init(const GDLUser& userData);
};