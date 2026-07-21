#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RangePopup : public geode::Popup {
public:
	static RangePopup* create(
		const char* title, int minValue, int maxValue,
		Function<void(int, int)> onApplyCallback
	);

	void setRange(int minValue, int maxValue);
	void setCallback(Function<void(int, int)> onApplyCallback);

protected:
	Function<void(int, int)> m_onApplyCallback = nullptr;

	int m_minValue;
	int m_maxValue;

	std::string m_previousFromValue;
	std::string m_previousToValue;

	CCMenu* m_menu;
	TextInput* m_fromTextInput;
	TextInput* m_toTextInput;

	bool init(
		const char* title, int minValue, int maxValue,
		Function<void(int, int)> onApplyCallback
	);
	void onApply(CCObject*);
	void onClose(CCObject*);
};