#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../../Models/GlobalListUserRecords.hpp"

using namespace geode::prelude;

class UserRecordsLoadedEvent : public Event<UserRecordsLoadedEvent, bool(GlobalListUserRecords*), int> {
public:
    using Event::Event;
};