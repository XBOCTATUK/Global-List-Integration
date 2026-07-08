#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListUserRecords.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class UserRecordsLoadedEvent : public Event<UserRecordsLoadedEvent, bool(Result<GlobalListUserRecords*, APIError>), int> {
public:
    using Event::Event;
};