#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../../Models/GlobalListUser.hpp"

using namespace geode::prelude;

class UserLeaderboardLoadedEvent : public Event<UserLeaderboardLoadedEvent, bool(std::vector<GlobalListUser>*)> {
public:
    using Event::Event;
};