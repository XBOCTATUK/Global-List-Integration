#pragma once

#include <Geode/Geode.hpp>

#include "../../events/PopulateListEvent.hpp"

#include "../../GlobalList/Filters.hpp"
#include "../../GlobalList/Cache/LevelCache.hpp"

#include "../LoadingPopup/LoadingPopup.hpp"
#include "../RangePopup/RangePopup.hpp"

using namespace geode::prelude;

class FilterPopup : public geode::Popup, LevelManagerDelegate {
public:
	static FilterPopup* create();

	ListenerHandle m_closeListener;
	CCMenuItemToggler* m_ratedFilterToggler;
	CCMenuItemToggler* m_unratedFilterToggler;

protected:
	bool init();
	void changeLengthFilter(LengthFilter type, CCLabelBMFont* label);
	void changeDiffFilter(DifficultyFilter type, CCLabelBMFont* label);
};