#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListLevel.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class DemonlistLoadedEvent : public Event<DemonlistLoadedEvent, bool(Result<std::vector<GDLLevel*>, APIError>)> {
public:
    using Event::Event;
};