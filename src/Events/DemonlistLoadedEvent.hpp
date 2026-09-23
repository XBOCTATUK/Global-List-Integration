#pragma once

#include "../Models/APIError.hpp"

class DemonlistLoadedEvent : public geode::Event<DemonlistLoadedEvent, bool(geode::Result<std::vector<int>, APIError>)> {
public:
    using Event::Event;
};