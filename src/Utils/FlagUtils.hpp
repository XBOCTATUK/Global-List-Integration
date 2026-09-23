#pragma once

#include "Geode/utils/string.hpp"
inline const std::vector<std::string> allCountries = {
    "All countries", "Andorra", "United-Arab-Emirates", "Afghanistan", "Albania", "Armenia", "Argentina",
    "Austria", "Australia", "Aruba", "Azerbaijan", "Bosnia-and-Herzegovina", "Bangladesh",
    "Belgium", "Bulgaria", "Bahrain", "Bolivia", "Brazil", "Belarus", "Canada",
    "Republic-of-the-Congo", "Switzerland", "Chile", "China", "Colombia", "Costa-Rica",
    "Cuba", "Cape-Verde", "Cyprus", "Czech-Republic", "Germany", "Denmark",
    "Dominican-Republic", "Algeria", "Ecuador", "Estonia", "Egypt", "Spain",
    "Finland", "France", "Gabon", "United-Kingdom", "England", "Scotland", "Wales",
    "Georgia", "Guernsey", "Ghana", "Gibraltar", "Greenland", "Greece", "Guatemala",
    "Guam", "Guyana", "Hong-Kong", "Honduras", "Croatia", "Haiti", "Hungary",
    "Canary-Islands", "Indonesia", "Ireland", "Israel", "Isle-of-Man", "India",
    "Iraq", "Iran", "Iceland", "Italy", "Jamaica", "Jordan", "Japan", "Kenya",
    "Kyrgyzstan", "Cambodia", "South-Korea", "Kuwait", "Kazakhstan", "Laos", "Lebanon",
    "Liechtenstein", "Lithuania", "Luxembourg", "Latvia", "Libya", "Morocco", "Moldova",
    "Montenegro", "Macedonia", "Mali", "Myanmar", "Mongolia", "Malta", "Maldives",
    "Mexico", "Malaysia", "Nigeria", "Nicaragua", "Netherlands", "Norway", "New-Zealand",
    "Oman", "Panama", "Peru", "Philippines", "Pakistan", "Poland", "Puerto-Rico",
    "Palestine", "Portugal", "Paraguay", "Qatar", "Romania", "Serbia", "Russia",
    "Saudi-Arabia", "Sweden", "Singapore", "Slovenia", "Slovakia", "Senegal",
    "Somalia", "El-Salvador", "Syria", "Togo", "Thailand", "Turkmenistan",
    "Tunisia", "Turkey", "Trinidad-and-Tobago", "Taiwan", "Ukraine", "United-States",
    "Uruguay", "Uzbekistan", "Saint-Vincent-and-the-Grenadines", "Venezuela", "Vietnam",
    "Kosovo", "Yemen", "South-Africa", "Zambia", "Tajikistan", "San-Marino",
    "Bahamas", "Unknown"
};

inline std::vector<std::string> allCountryNames;

namespace Utils {
    inline const std::string getCountrySpriteName(const std::string& flagName) {
        auto it = std::find(allCountries.begin(), allCountries.end(), flagName);
        if (it == allCountries.end()) return "Unknown.png"_spr;
        
        return fmt::format("{}.png"_spr, flagName).c_str();
    }

    inline const std::vector<std::string>& getCountryNames() {
        if (allCountryNames.empty()) {
            for (auto country : allCountries) {
                if (country == "Unknown") continue;
                allCountryNames.push_back(geode::utils::string::replace(country, "-", " "));
            }
        }

        return allCountryNames;
    }
}