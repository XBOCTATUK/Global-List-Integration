#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class UserCachedEvent : public Event<UserCachedEvent, bool(), std::string> {
public:
    using Event::Event;
};