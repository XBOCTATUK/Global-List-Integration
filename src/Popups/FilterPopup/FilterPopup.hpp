#pragma once

#include "../../UI/OptionBar/OptionBar.hpp"
#include "../../UI/LabeledCheckbox/LabeledCheckbox.hpp"

class FilterPopup : public geode::Popup, LevelManagerDelegate {
public:
	static FilterPopup* create();

protected:
    CCNode* m_filterContainer = nullptr;

	geode::ListenerHandle m_closeListener;

    TailyUI::OptionBar* m_diffFilterBar = nullptr;
    TailyUI::OptionBar* m_lengthFilterBar = nullptr;
    CCNode* m_togglerContainer = nullptr;

	TailyUI::LabeledCheckbox* m_ratedToggler = nullptr;
	TailyUI::LabeledCheckbox* m_unratedToggler = nullptr;
	TailyUI::LabeledCheckbox* m_completedToggler = nullptr;
	TailyUI::LabeledCheckbox* m_createdByToggler = nullptr;

    geode::TextInput* m_usernameInput = nullptr;
    geode::TextInput* m_creatorNameInput = nullptr;

	bool init();
};