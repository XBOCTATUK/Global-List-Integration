#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class CloseFiltersEvent : public Event<CloseFiltersEvent, bool()> {
public:
    using Event::Event;
};