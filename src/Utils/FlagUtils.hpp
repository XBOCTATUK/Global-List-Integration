#pragma once

#include "Geode/utils/string.hpp"
inline const std::vector<std::string> allCountries = {
    "All countries", "United-States", "New-Zealand", "Russia", "Armenia", "Finland", "Spain",
    "South-Korea", "Portugal", "Brazil", "Kazakhstan", "Moldova", "Poland", "Canada",
    "Germany", "Chile", "Peru", "United-Kingdom", "Australia", "Japan", "Romania", "Norway",
    "Netherlands", "Ukraine", "Denmark", "Austria", "Israel", "Uzbekistan", "Estonia", "Italy",
    "France", "Belgium", "Serbia", "China", "Colombia", "Argentina", "Panama", "Albania",
    "Czech-Republic", "Sweden", "Lithuania", "Philippines", "Hungary", "Belarus", "Mexico",
    "India", "Venezuela", "Pakistan", "Ireland", "Indonesia", "United-Arab-Emirates", "Nigeria",
    "Georgia", "Slovakia", "Iceland", "Costa-Rica", "Laos", "Bosnia-and-Herzegovina",
    "South-Africa", "Kosovo", "Latvia", "Bulgaria", "Slovenia", "Guernsey", "Ecuador",
    "Vietnam", "Cambodia", "Iran", "Dominican-Republic", "Malta", "Turkmenistan", "Algeria",
    "Palestine", "Egypt", "Greece", "Cuba", "Turkey", "Taiwan", "Paraguay", "Tunisia",
    "Kyrgyzstan", "Croatia", "Azerbaijan", "Morocco", "Bolivia", "Switzerland", "Hong-Kong",
    "Iraq", "Uruguay", "Nicaragua", "Luxembourg", "Thailand", "Canary-Islands", "Jamaica",
    "Guatemala", "Ghana", "Puerto-Rico", "Montenegro", "Somalia", "Mongolia", "El-Salvador",
    "Syria", "Greenland", "Saudi-Arabia", "Afghanistan", "Malaysia", "Macedonia", "Bahrain",
    "Maldives", "Libya", "Senegal", "Cape-Verde", "Guyana", "Honduras",
    "Saint-Vincent-and-the-Grenadines", "Trinidad-and-Tobago", "Guam", "Wales", "Cyprus",
    "Haiti", "Singapore", "Yemen", "Lebanon", "Oman", "Gibraltar", "England", "Kenya",
    "Kuwait", "Aruba", "Jordan", "Andorra", "San-Marino", "Myanmar", "Liechtenstein",
    "Scotland", "Bahamas", "Mali", "Togo", "Zambia", "Tajikistan", "Isle-of-Man", "Bangladesh",
    "Republic-of-the-Congo", "Gabon", "Qatar", "Unknown"
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