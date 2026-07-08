#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GDLUser.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class UserLoadedEvent : public Event<UserLoadedEvent, bool(Result<GDLUser*, APIError>), int> {
public:
    using Event::Event;
};