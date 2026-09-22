#pragma once

#include "../../UI/OptionBar/OptionBar.hpp"
#include "../../UI/LabeledCheckbox/LabeledCheckbox.hpp"

class FilterPopup : public geode::Popup, LevelManagerDelegate {
public:
	static FilterPopup* create();

protected:
    CCNode* m_filterContainer;

	geode::ListenerHandle m_closeListener;

    TailyUI::OptionBar* m_diffFilterBar;
    TailyUI::OptionBar* m_lengthFilterBar;
    CCNode* m_togglerContainer;

	TailyUI::LabeledCheckbox* m_ratedToggler;
	TailyUI::LabeledCheckbox* m_unratedToggler;
	TailyUI::LabeledCheckbox* m_completedToggler;
	TailyUI::LabeledCheckbox* m_createdByToggler;

    geode::TextInput* m_usernameInput;
    geode::TextInput* m_creatorNameInput;

	bool init();
};