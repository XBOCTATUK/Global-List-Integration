#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

enum class PopupType {
    FilterPopup
};

class MyCloseEvent : public Event<MyCloseEvent, bool(), PopupType> {
public:
    using Event::Event;
};