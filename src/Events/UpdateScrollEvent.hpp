#pragma once

class UpdateScrollEvent : public geode::Event<UpdateScrollEvent, bool()> {
public:
    using Event::Event;
};