#pragma once

#include <Geode/Geode.hpp>
#include "../../GlobalList/Filters.hpp"

using namespace geode::prelude;

enum class FilterType {
	Lenght, Difficulty
};

class RangePopup : public geode::Popup {
public:
	static RangePopup* create(FilterType type);

protected:
	FilterType m_filterType;
	TextInput* m_fromTextInput;
	TextInput* m_toTextInput;

	bool init(FilterType type);
	virtual void onClose(cocos2d::CCObject*) override;
};