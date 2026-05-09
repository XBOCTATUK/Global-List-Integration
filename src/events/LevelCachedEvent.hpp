#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class LevelsCachedEvent : public Event<LevelsCachedEvent, bool(), int> {
public:
    using Event::Event;
};