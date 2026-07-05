#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../../Models/GlobalListLevel.hpp"

using namespace geode::prelude;

class LevelLoadedEvent : public Event<LevelLoadedEvent, bool(GlobalListLevel*), int> {
public:
    using Event::Event;
};