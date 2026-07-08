#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListUser.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class UserLoadedEvent : public Event<UserLoadedEvent, bool(Result<GlobalListUser*, APIError>), int> {
public:
    using Event::Event;
};