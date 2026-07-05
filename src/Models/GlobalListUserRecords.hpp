#pragma once

#include <vector>
#include "GlobalListRecord.hpp"

struct GlobalListUserRecords {
    int userID;
    int totalCount;
    int completedCount;
    std::vector<GlobalListRecord> records;
};