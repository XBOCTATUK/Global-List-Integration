#include "CountryInfoPopup.hpp"

using namespace geode::prelude;

CountryInfoPopup* CountryInfoPopup::create(const GDLCountry& countryUser) {
	auto ret = new CountryInfoPopup();
	if (ret && ret->init(countryUser)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool CountryInfoPopup::init(const GDLCountry& countryUser) {
    if (!Popup::init(450.0f, 280.0f)) return false;

    return true;
}