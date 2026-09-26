#pragma once

enum class CountriesLeaderboardType { Main, Advanced };

struct GDLCountry {
    std::string title;
    int placement = 0;
    double points = 0.0;
};