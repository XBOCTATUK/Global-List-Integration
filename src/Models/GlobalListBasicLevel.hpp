#pragma once

#include <string>
#include <optional>

struct GlobalListBasicLevel {
    int id;
    std::string name;
    int placement;
    std::string videoURL;
    std::optional<int> percent;
};