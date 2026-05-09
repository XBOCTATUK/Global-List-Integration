#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class LeaderboardsCachedEvent : public Event<LeaderboardsCachedEvent, bool()> {
public:
    using Event::Event;
};