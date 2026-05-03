#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event dispatcher for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event_listener.h"
#include "event.h"
#include "internal/read_write_mutex.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

namespace vne::events {

/**
 * @class EventDispatcher
 * @brief Handles event registration, unregistration, and dispatching.
 *
 * The dispatcher maintains a map of event types to listener lists and
 * dispatches events to all registered listeners in a thread-safe manner.
 *
 * @threadsafe All public methods are thread-safe.
 */
class VNEEVENTS_API EventDispatcher {
   public:
    using ListenerPtr = std::shared_ptr<EventListener>;

    EventDispatcher() = default;
    ~EventDispatcher() = default;

    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;

    /**
     * @brief Registers an event listener for a specific event type.
     * @param event_type The event type to register for.
     * @param listener The listener to be registered.
     */
    void registerListener(EventType event_type, ListenerPtr listener) {
        internal::WriteLockGuard lock(mutex_);
        listeners_[event_type].push_back(std::move(listener));
    }

    /**
     * @brief Unregisters an event listener.
     * @param event_type The event type to unregister from.
     * @param listener The listener to be unregistered.
     */
    void unregisterListener(EventType event_type, const EventListener* listener) {
        internal::WriteLockGuard lock(mutex_);
        auto it = listeners_.find(event_type);
        if (it != listeners_.end()) {
            auto& list = it->second;
            list.erase(std::remove_if(list.begin(),
                                      list.end(),
                                      [listener](const ListenerPtr& l) { return l.get() == listener; }),
                       list.end());
        }
    }

    /**
     * @brief Dispatches an event to all registered listeners.
     * @param event The event to be dispatched.
     */
    void dispatch(const Event& event) const {
        std::vector<ListenerPtr> listeners_copy;
        {
            internal::ReadLockGuard lock(mutex_);
            auto it = listeners_.find(event.type());
            if (it != listeners_.end()) {
                listeners_copy = it->second;
            }
        }
        // Invoke callbacks outside the lock to minimize contention
        for (const auto& listener : listeners_copy) {
            listener->onEvent(event);
        }
    }

    /**
     * @brief Gets the number of listeners for a specific event type.
     * @param event_type The event type to query.
     * @return The number of registered listeners.
     */
    [[nodiscard]] size_t listenerCount(EventType event_type) const {
        internal::ReadLockGuard lock(mutex_);
        auto it = listeners_.find(event_type);
        return it != listeners_.end() ? it->second.size() : 0;
    }

   private:
    std::unordered_map<EventType, std::vector<ListenerPtr>> listeners_;
    mutable internal::ReadWriteMutex mutex_;
};

}  // namespace vne::events
