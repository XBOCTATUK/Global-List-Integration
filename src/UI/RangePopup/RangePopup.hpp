#pragma once

namespace TailyUI {
	class RangePopup : public geode::Popup {
	public:
		static RangePopup* create(
			const char* title, int minValue, int maxValue,
			geode::Function<void(int, int)> onApplyCallback
		);

		void setRange(int minValue, int maxValue);
		void setCallback(geode::Function<void(int, int)> onApplyCallback);

	protected:
		geode::Function<void(int, int)> m_onApplyCallback = nullptr;

		int m_minValue = 0;
		int m_maxValue = INT_MAX;

		std::string m_previousFromValue;
		std::string m_previousToValue;

		cocos2d::CCMenu* m_menu = nullptr;
		geode::TextInput* m_fromTextInput = nullptr;
		geode::TextInput* m_toTextInput = nullptr;

		bool init(
			const char* title, int minValue, int maxValue,
			geode::Function<void(int, int)> onApplyCallback
		);
		void onApply(CCObject*);
		void onClose(CCObject*);
	};
}