#include "../../Models/GDLCountry.hpp"

class CountryInfoPopup : public geode::Popup {
public:
	static CountryInfoPopup* create(const GDLCountry& countryData, CountriesLeaderboardType type);

protected:
	GDLCountry m_countryData{};
	CountriesLeaderboardType m_type = CountriesLeaderboardType::Main;
	geode::ListenerHandle m_countryLoadListener;

	geode::ScrollLayer* m_scrollLayer = nullptr;

	bool init(const GDLCountry& countryData, CountriesLeaderboardType type);
	void drawBasicInfoUI();
	void drawMainTypeUI();
	void drawAdvancedTypeUI();
};