#pragma once

#include <string>
#include <optional>

struct GDLBasicLevel {
    int id;
    std::string name;
    int placement;
    std::string videoURL;
    std::optional<int> percent;
};