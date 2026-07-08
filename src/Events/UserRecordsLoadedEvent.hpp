#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GDLUserRecords.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class UserRecordsLoadedEvent : public Event<UserRecordsLoadedEvent, bool(Result<GDLUserRecords*, APIError>), int> {
public:
    using Event::Event;
};