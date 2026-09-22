#pragma once

#include "../Models/GDLUserRecords.hpp"
#include "../Models/APIError.hpp"

class UserRecordsLoadedEvent : public geode::Event<UserRecordsLoadedEvent, bool(geode::Result<const GDLUserRecords*, APIError>), int> {
public:
    using Event::Event;
};