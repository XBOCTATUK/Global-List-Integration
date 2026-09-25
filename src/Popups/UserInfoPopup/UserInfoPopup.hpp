#include "../../Models/GDLUser.hpp"

class UserInfoPopup : public geode::Popup {
public:
	static UserInfoPopup* create(const GDLUser& userData);

protected:
	GDLUser m_userData;
	geode::ListenerHandle m_userLoadListener;

	cocos2d::CCMenu* m_basicInfoMenu;
	cocos2d::CCLabelBMFont* m_usernameLabel;
	CCMenuItemSpriteExtra* m_badgeBtn;
    CCMenuItemSpriteExtra* m_flagBtn;

	

	bool init(const GDLUser& userData);
	void drawUI();
	CCNode* createStatNode(const std::string& text, const std::string& subtext, const std::string& icon, float width);
	CCNode* createUserLevelsNode(const OptGDLBasicLevels& levels, const std::string& text, const std::string& icon);
	CCNode* createLevelButtonSprite(const GDLBasicLevel& level);
};