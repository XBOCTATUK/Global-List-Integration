#pragma once

#include <vector>
#include "GDLRecord.hpp"

struct GDLUserRecords {
    int userID = 0;
    int totalCount = 0;
    int completedCount = 0;
    std::vector<GDLRecord> records;
};