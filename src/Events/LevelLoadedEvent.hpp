#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListLevel.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class LevelLoadedEvent : public Event<LevelLoadedEvent, bool(Result<GDLLevel*, APIError>), int> {
public:
    using Event::Event;
};