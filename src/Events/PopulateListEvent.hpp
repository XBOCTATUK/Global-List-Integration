#pragma once

class PopulateListEvent : public geode::Event<PopulateListEvent, bool()> {
public:
    using Event::Event;
};