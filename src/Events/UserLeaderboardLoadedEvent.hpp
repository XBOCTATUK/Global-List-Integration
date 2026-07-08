#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "../Models/GlobalListUser.hpp"
#include "../Models/APIError.hpp"

using namespace geode::prelude;

class UserLeaderboardLoadedEvent : public Event<UserLeaderboardLoadedEvent, bool(Result<std::vector<GlobalListUser*>, APIError>)> {
public:
    using Event::Event;
};