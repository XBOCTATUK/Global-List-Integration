#include "../../Models/GDLCountry.hpp"

class CountryInfoPopup : public geode::Popup {
public:
	static CountryInfoPopup* create(const GDLCountry& countryUser);

protected:

	bool init(const GDLCountry& countryUser);
};