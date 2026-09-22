#include "SearchBar.hpp"

#include "../../Popups/FilterPopup/FilterPopup.hpp"
#include "../../Utils/FlagUtils.hpp"
#include "Geode/c++stl/string.hpp"
#include <cue/ListNode.hpp>

using namespace geode::prelude;

namespace TailyUI {
    SearchBar* SearchBar::create(ZStringView placeholder, SearchBarType type, geode::Function<void(gd::string)> callback) {
    	auto ret = new SearchBar();
        if (ret && ret->init(placeholder, type, std::move(callback))) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

	bool SearchBar::init(ZStringView placeholder, SearchBarType type, geode::Function<void(gd::string)> searchCallback) {
		if (!CCNode::init()) return false;

		setSearchCallback(std::move(searchCallback));
		setContentSize({ 356.0f, 30.0f });

		auto searchBarBG = CCLayerColor::create({ 194, 114, 62, 255 }, getContentWidth(), getContentHeight());
		addChild(searchBarBG);

		m_searchBarMenu = CCMenu::create();
		m_searchBarMenu->setLayout(
			RowLayout::create()
			->setGap(5.0f)
			->setAutoScale(false)
			->setCrossAxisOverflow(false)
		);
		m_searchBarMenu->setContentSize(getContentSize());
		m_searchBarMenu->setAnchorPoint({ 0.0f, 0.0f });
		m_searchBarMenu->setPosition({ 0.0f, 0.0f });
		m_searchBarMenu->setTouchPriority(-256);
		m_searchBarMenu->setUserObject("gdl-block-touches", CCBool::create(true));
		addChild(m_searchBarMenu, 5);

		float width = type == SearchBarType::WithFilters
		? 380.0f
		: type == SearchBarType::WithCountrySelection
		? 255.0f : 300.0f;

		m_searchInput = TextInput::create(width, placeholder);
		m_searchInput->setMaxCharCount(32);
		m_searchInput->setTextAlign(TextInputAlign::Left);
		m_searchInput->getInputNode()->setLabelPlaceholderScale(0.70f);
		m_searchInput->getInputNode()->setMaxLabelScale(0.70f);
		m_searchInput->setScale(0.75f);
		m_searchBarMenu->addChild(m_searchInput, 1);

		auto findIcon = CCSprite::create("findIcon.png"_spr);
		auto searchSpr = CCSprite::create("GJ_button_01.png");
		searchSpr->addChildAtPosition(findIcon, Anchor::Center, {});
		searchSpr->setScale(22.5f / searchSpr->getContentHeight());

		m_searchBtn = CCMenuItemExt::createSpriteExtra(
			searchSpr, [this](auto) {
				if (m_searchCallback) {
					m_searchCallback(m_searchInput->getString());
				}
			}
		);
		m_searchBarMenu->addChild(m_searchBtn, 3);

		setSearchBarType(type);

		return true;
	}

	geode::TextInput* SearchBar::getSearchInput() {
		return m_searchInput;
	}

	TailyUI::DropdownList* SearchBar::getDropdownList() {
		if (m_dropdown) return m_dropdown;

		return nullptr;
	}

	void SearchBar::setSearchCallback(geode::Function<void(gd::string)> callback) {
		m_searchCallback = std::move(callback);
	}

	void SearchBar::setSearchBarType(SearchBarType type) {
		float searchInputWidth =
			type == SearchBarType::WithFilters ? 380.0f :
			type == SearchBarType::WithCountrySelection ? 255.0f :
			300.0f;

		m_searchInput->setWidth(searchInputWidth);
		m_searchInput->updateLayout();

		if (type == SearchBarType::WithFilters) {
			if (!m_filterBtn) {
				auto filterIcon = CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png");
				auto filterSpr = CCSprite::create("GJ_button_01.png");
				filterSpr->addChildAtPosition(filterIcon, Anchor::Center, {});
				filterSpr->setScale(22.5f / filterSpr->getContentHeight());

				m_filterBtn = CCMenuItemExt::createSpriteExtra(
					filterSpr, [this](auto) {
						FilterPopup::create()->show();
					}
				);

				m_searchBarMenu->addChild(m_filterBtn, 2);
			}

			m_filterBtn->setVisible(true);

			if (m_dropdown) {
				m_dropdown->setVisible(false);
			}
		}
		else {
			if (!m_dropdown) {
				auto values = type == SearchBarType::WithCountrySelection
					? Utils::getCountryNames()
					: std::vector<std::string>{"Main", "Advanced"};

				float dropdownWidth = type == SearchBarType::WithCountrySelection
					? 160.0f
					: 115.0f;

				m_dropdown = TailyUI::DropdownList::create(
					values,
					dropdownWidth,
					4,
					[this](size_t idx, const std::string& value) {
						if (m_dropdownCallback) {
							m_dropdownCallback(idx, value);
						}
					}
				);

				m_dropdown->setScale(0.75f);
				m_searchBarMenu->addChild(m_dropdown, 2);
			}
			else {
				float dropdownWidth = type == SearchBarType::WithCountrySelection
					? 160.0f
					: 115.0f;

				m_dropdown->setContentWidth(dropdownWidth);

				if (type == SearchBarType::WithCountrySelection) {
					m_dropdown->setValues(Utils::getCountryNames());
				}
				else {
					m_dropdown->setValues({ "Main", "Advanced" });
				}
			}

			m_dropdown->setVisible(true);

			if (m_filterBtn) {
				m_filterBtn->setVisible(false);
			}
		}

		m_searchBarMenu->updateLayout();
	}

	void SearchBar::draw() {
		CCNode::draw();

		// i stole this from cue
		auto size = getContentSize();
		cocos2d::ccDrawColor4B(0, 0, 0, 0x4f);
		glLineWidth(2.0f);
		cocos2d::ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f });
		cocos2d::ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height });
	}
}