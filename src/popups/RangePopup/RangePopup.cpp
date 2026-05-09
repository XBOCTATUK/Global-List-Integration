#include "./RangePopup.hpp"

RangePopup* RangePopup::create(FilterType type) {
	auto ret = new RangePopup();
	if (ret && ret->init(type)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool RangePopup::init(FilterType type) {
	if (!Popup::init(200.0f, 120.0f)) return false;

	this->setID("range-menu");
	this->setZOrder(102);
	this->setTitle(type == FilterType::Lenght ? "Custom length" : "Custom difficulty");

	m_closeBtn->setVisible(false);
	m_filterType = type;
	auto& levelFilters = GlobalList::Filters::getLevelFilters();
	auto& saved = type == FilterType::Lenght ? levelFilters.customLengthFilter : levelFilters.customDiffFilter;

	auto label = CCLabelBMFont::create("From         to        ", "bigFont.fnt");
	label->setScale(0.45f);
	label->setPosition({ 100.0f, 64.0f });
	m_mainLayer->addChild(label);

	m_fromTextInput = TextInput::create(45.0f, "");
	m_fromTextInput->setPosition({ 83.0f, 64.0f });
	m_fromTextInput->setMaxCharCount(4);
	m_fromTextInput->setCommonFilter(CommonFilter::Uint);
	m_fromTextInput->setString(saved[0] != 0 ? std::to_string(saved[0]) : "");

	m_toTextInput = TextInput::create(45.0f, "");
	m_toTextInput->setPosition({ 161.0f, 64.0f });
	m_toTextInput->setMaxCharCount(4);
	m_toTextInput->setCommonFilter(CommonFilter::Uint);
	m_toTextInput->setString(saved[1] != 0 ? std::to_string(saved[1]) : "");

	auto okSpr = ButtonSprite::create("Ok");
	auto okBtn = CCMenuItemExt::createSpriteExtra(okSpr, [this, type](auto) {
		auto& levelFilters = GlobalList::Filters::getLevelFilters();
		int from = numFromString<int>(m_fromTextInput->getString().size() != 0 ? m_fromTextInput->getString() : "0").unwrapOrDefault();
		int to = numFromString<int>(m_toTextInput->getString().size() != 0 ? m_toTextInput->getString() : "0").unwrapOrDefault();

		if (type == FilterType::Lenght) GlobalList::Filters::setCustomLengthFilter(from, to);
		else GlobalList::Filters::setCustomDifficultyFilter(from, to);

		Popup::onClose(this);
	});
	okBtn->setPosition({ 100.0f, 24.0f });

	m_buttonMenu->addChild(m_fromTextInput);
	m_buttonMenu->addChild(m_toTextInput);
	m_buttonMenu->addChild(okBtn);

	return true;
}

void RangePopup::onClose(cocos2d::CCObject*) {
	int from = numFromString<int>(m_fromTextInput->getString().size() != 0 ? m_fromTextInput->getString() : "0").unwrapOrDefault();
	int to = numFromString<int>(m_toTextInput->getString().size() != 0 ? m_toTextInput->getString() : "0").unwrapOrDefault();

	auto& levelFilters = GlobalList::Filters::getLevelFilters();
	auto& saved = m_filterType == FilterType::Lenght ? levelFilters.customLengthFilter : levelFilters.customDiffFilter;
	if (from != saved[0] || to != saved[1]) {
		createQuickPopup("Hey!", "Fields not saved. Save and close?", "No", "Yes", [this, &saved, from, to](auto, bool yesBtn) {
			if (yesBtn) {
				if (m_filterType == FilterType::Lenght) GlobalList::Filters::setCustomLengthFilter(from, to);
				else GlobalList::Filters::setCustomDifficultyFilter(from, to);

				Popup::onClose(this);
			}
		});
	}
	else Popup::onClose(this);
}