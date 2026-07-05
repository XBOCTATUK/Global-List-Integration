#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListUser.hpp"

using namespace geode::prelude;

class UserLoadedEvent : public Event<UserLoadedEvent, bool(GlobalListUser*), int> {
public:
    using Event::Event;
};