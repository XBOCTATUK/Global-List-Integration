#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class PositionEvent : public Event<PositionEvent, bool(int), int> {
public:
    using Event::Event;
};