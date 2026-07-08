#pragma once

#include <string>

enum class CountriesLeaderboardType { Main, Advanced };

struct GlobalListCountry {
    std::string title;
    int placement;
    double points;
};