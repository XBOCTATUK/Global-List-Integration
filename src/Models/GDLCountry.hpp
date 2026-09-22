#pragma once

enum class CountriesLeaderboardType { Main, Advanced };

struct GDLCountry {
    std::string title;
    int placement;
    double points;
};