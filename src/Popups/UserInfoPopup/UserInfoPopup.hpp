#include "../../Models/GDLUser.hpp"

class UserInfoPopup : public geode::Popup {
public:
	static UserInfoPopup* create(int userID);

protected:
	int m_userID;
	geode::ListenerHandle m_userLoadListener;

	cocos2d::CCMenu* m_basicInfoMenu;
	cocos2d::CCLabelBMFont* m_usernameLabel;
	CCMenuItemSpriteExtra* m_badgeBtn;
    CCMenuItemSpriteExtra* m_flagBtn;

	bool init(int userID);
	void drawUI();
};