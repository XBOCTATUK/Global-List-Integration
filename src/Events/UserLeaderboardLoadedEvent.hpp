#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class UserLeaderboardLoadedEvent : public Event<UserLeaderboardLoadedEvent, bool(Result<const std::vector<int>*, APIError>)> {
public:
    using Event::Event;
};