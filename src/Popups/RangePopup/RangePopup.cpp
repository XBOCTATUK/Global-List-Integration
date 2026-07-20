#include "RangePopup.hpp"

RangePopup* RangePopup::create(
	const char* title, int minValue, int maxValue,
	Function<void(int, int)> onApplyCallback
) {
	auto ret = new RangePopup();
	if (ret && ret->init(title, minValue, maxValue, std::move(onApplyCallback))) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool RangePopup::init(
	const char* title, int minValue, int maxValue,
	Function<void(int, int)> onApplyCallback
) {
	if (!Popup::init(200.0f, 120.0f)) return false;

	setID("range-menu");
	setZOrder(102);
	setTitle(title);

	m_closeBtn->setVisible(false);

	m_menu = CCMenu::create();
	m_menu->setLayout(
		RowLayout::create()
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false)
	);
	m_menu->setPosition({ m_mainLayer->getContentWidth() / 2.0f, 68.0f });
	m_menu->setContentSize({ m_mainLayer->getContentWidth() - 40.0f, 40.0f });
	m_menu->setID("range-input");
	m_mainLayer->addChild(m_menu);

	auto fromLabel = CCLabelBMFont::create("From", "bigFont.fnt");
	fromLabel->setScale(0.5f);
	fromLabel->setID("from-text");
	m_menu->addChild(fromLabel);

	m_fromTextInput = TextInput::create(45.0f, "");
	m_fromTextInput->setScale(0.8f);
	m_fromTextInput->setCallback(
		[this](const std::string& text) {
			if (text.empty()) {
				m_previousFromValue.clear();
				return;
			}
			else if (text == "-") return;

			if (text.find('-', 1) != std::string::npos) {
				m_fromTextInput->setString(m_previousFromValue);
				return;
			}

			auto from = numFromString<int>(text);
			if (from.isErr()) return;

			auto fromValue = from.unwrap();
			if (fromValue < m_minValue || fromValue > m_maxValue) {
				m_fromTextInput->setString(m_previousFromValue);
				return;
			}

			m_previousFromValue = text;
		}
	);
	m_fromTextInput->setID("from-input");
	m_menu->addChild(m_fromTextInput);

	auto toLabel = CCLabelBMFont::create("to", "bigFont.fnt");
	toLabel->setScale(0.5f);
	toLabel->setID("to-text");
	m_menu->addChild(toLabel);

	m_toTextInput = TextInput::create(45.0f, "");
	m_toTextInput->setScale(0.8f);
	m_toTextInput->setCallback(
		[this](const std::string& text) {
			if (text.empty()) {
				m_previousToValue.clear();
				return;
			}
			else if (text == "-") return;

			if (text.find('-', 1) != std::string::npos) {
				m_toTextInput->setString(m_previousToValue);
				return;
			}

			auto to = numFromString<int>(text);
			if (to.isErr()) return;

			auto toValue = to.unwrap();
			if (toValue < m_minValue || toValue > m_maxValue) {
				m_toTextInput->setString(m_previousToValue);
				return;
			}

			m_previousToValue = text;
		}
	);
	m_toTextInput->setID("to-input");
	m_menu->addChild(m_toTextInput);

	m_menu->updateLayout();

	auto applySpr = ButtonSprite::create("Apply", 0.8f);
	auto applyBtn = CCMenuItemSpriteExtra::create(
		applySpr, this, menu_selector(RangePopup::onApply)
	);
	applyBtn->setPosition({ 100.0f, 28.0f });
	applyBtn->setID("apply-button");
	m_buttonMenu->addChild(applyBtn);

	setRange(minValue, maxValue);
	setCallback(std::move(onApplyCallback));

	return true;
}

void RangePopup::setRange(int minValue, int maxValue) {
	m_minValue = minValue;
	m_maxValue = maxValue;

	if (minValue >= 0) {
		m_fromTextInput->setCommonFilter(CommonFilter::Uint);
		m_toTextInput->setCommonFilter(CommonFilter::Uint);
	}
	else {
		m_fromTextInput->setCommonFilter(CommonFilter::Int);
		m_toTextInput->setCommonFilter(CommonFilter::Int);
	}
}

void RangePopup::setCallback(Function<void(int, int)> onApplyCallback) {
	m_onApplyCallback = std::move(onApplyCallback);
}

void RangePopup::onApply(CCObject*) {
	int from = numFromString<int>(
		!m_fromTextInput->getString().empty() ? m_fromTextInput->getString() : "0"
	).unwrapOrDefault();

	int to = numFromString<int>(
		!m_toTextInput->getString().empty() ? m_toTextInput->getString() : "0"
	).unwrapOrDefault();

	if (m_onApplyCallback) {
		m_onApplyCallback(from, to);
	}

	Popup::onClose(nullptr);
}

void RangePopup::onClose(CCObject*) {
	if (!m_fromTextInput->getString().empty() || !m_toTextInput->getString().empty()) {
		createQuickPopup(
			"Hey!", "Fields not saved. Save and close?", "No", "Yes",
			[this](auto, bool yesBtn) {
				if (yesBtn) {
					onApply(nullptr);
				}
			}
		);
	}
	else {
		Popup::onClose(nullptr);
	}
}