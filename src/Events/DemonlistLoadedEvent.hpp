#pragma once

#include "../Models/APIError.hpp"

class DemonlistLoadedEvent : public geode::Event<DemonlistLoadedEvent, bool(geode::Result<const std::vector<int>&, APIError>)> {
public:
    using Event::Event;
};