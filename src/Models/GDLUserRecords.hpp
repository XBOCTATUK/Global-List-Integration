#pragma once

#include <vector>
#include "GDLRecord.hpp"

struct GDLUserRecords {
    int userID;
    int totalCount;
    int completedCount;
    std::vector<GDLRecord> records;
};