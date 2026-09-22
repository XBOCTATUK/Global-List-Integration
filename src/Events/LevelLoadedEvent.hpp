#pragma once

#include "../Models/GDLLevel.hpp"
#include "../Models/APIError.hpp"

class LevelLoadedEvent : public geode::Event<LevelLoadedEvent, bool(geode::Result<const GDLLevel*, APIError>), int> {
public:
    using Event::Event;
};