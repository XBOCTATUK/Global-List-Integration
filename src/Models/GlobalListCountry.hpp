#pragma once

#include <string>

enum CountriesLeaderboardType { Main, Advanced };

struct GlobalListCountry {
    std::string title;
    int placement;
    double points;
};