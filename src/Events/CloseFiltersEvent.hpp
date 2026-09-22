#pragma once

class CloseFiltersEvent : public geode::Event<CloseFiltersEvent, bool()> {
public:
    using Event::Event;
};