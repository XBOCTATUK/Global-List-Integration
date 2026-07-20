#pragma once

#include <Geode/Geode.hpp>

#include "../LoadingPopup/LoadingPopup.hpp"
#include "../RangePopup/RangePopup.hpp"
#include "../../Filters/Filters.hpp"
#include "../../Cache/GameLevels/GameLevels.hpp"
#include "../../UI/OptionBar/OptionBar.hpp"
#include "../../UI/LabeledCheckbox/LabeledCheckbox.hpp"
#include "../../Events/DemonlistLoadedEvent.hpp"
#include "../../Events/PopulateListEvent.hpp"
#include "../../Events/CloseFiltersEvent.hpp"

using namespace geode::prelude;

class FilterPopup : public Popup, LevelManagerDelegate {
public:
	static FilterPopup* create();

protected:
    CCNode* m_filterContainer;

	ListenerHandle m_closeListener;

    TailyUI::OptionBar* m_diffFilterBar;
    TailyUI::OptionBar* m_lengthFilterBar;
    CCNode* m_togglerContainer;

	TailyUI::LabeledCheckbox* m_ratedToggler;
	TailyUI::LabeledCheckbox* m_unratedToggler;
	TailyUI::LabeledCheckbox* m_completedToggler;
	TailyUI::LabeledCheckbox* m_createdByToggler;

    TextInput* m_usernameInput;
    TextInput* m_creatorNameInput;

	bool init();
};