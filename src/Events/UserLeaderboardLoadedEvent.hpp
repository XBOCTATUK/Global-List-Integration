#pragma once

#include "../Models/APIError.hpp"

class UserLeaderboardLoadedEvent : public geode::Event<UserLeaderboardLoadedEvent, bool(geode::Result<std::vector<int>, APIError>)> {
public:
    using Event::Event;
};