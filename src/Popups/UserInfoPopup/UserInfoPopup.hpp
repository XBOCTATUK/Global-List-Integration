#include "../../Models/GDLUser.hpp"

class UserInfoPopup : public geode::Popup {
public:
	static UserInfoPopup* create(int userID);

protected:
	int m_userID = 0;
	geode::ListenerHandle m_userLoadListener;

	cocos2d::CCMenu* m_basicInfoMenu = nullptr;
	cocos2d::CCLabelBMFont* m_usernameLabel = nullptr;
	CCMenuItemSpriteExtra* m_badgeBtn = nullptr;
    CCMenuItemSpriteExtra* m_flagBtn = nullptr;

	bool init(int userID);
	void drawUI();
};