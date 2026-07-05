#pragma once

#include <chrono>

namespace GlobalList::Cache {
    template <typename T>
    struct CacheEntry {
        T value;
        std::chrono::steady_clock::time_point cachedAt;
    };

    inline bool isExpired(std::chrono::steady_clock::time_point cachedAt, std::chrono::seconds ttl) {
        return std::chrono::steady_clock::now() - cachedAt > ttl;
    }
};