#pragma once

#include "../Models/GDLUser.hpp"
#include "../Models/APIError.hpp"

class UserLoadedEvent : public geode::Event<UserLoadedEvent, bool(geode::Result<const GDLUser*, APIError>), int> {
public:
    using Event::Event;
};