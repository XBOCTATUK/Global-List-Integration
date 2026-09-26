#pragma once

struct GDLBasicLevel {
    int id = 0;
    std::string name;
    int placement = 0;
    std::string videoURL;
    std::optional<int> percent;
};