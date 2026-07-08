#pragma once

#include <string>

enum class CountriesLeaderboardType { Main, Advanced };

struct GDLCountry {
    std::string title;
    int placement;
    double points;
};