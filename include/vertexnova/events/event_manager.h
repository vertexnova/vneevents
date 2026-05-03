#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event manager singleton for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event_bus.h"
#include "event_queue.h"
#include "event_listener.h"
#include "event.h"

#include <memory>

namespace vne::events {

/**
 * @class EventManager
 * @brief Singleton that manages event listeners and event processing.
 *
 * The EventManager encapsulates an EventBus and EventQueue, providing
 * a high-level interface for the event system.
 *
 * @threadsafe All public methods are thread-safe. The singleton instance
 *             is created using a thread-safe pattern.
 */
class VNEEVENTS_API EventManager {
   public:
    using ListenerPtr = std::shared_ptr<EventListener>;

    /**
     * @brief Gets the singleton instance of the EventManager.
     * @return Reference to the EventManager instance.
     *
     * @threadsafe This function is thread-safe.
     */
    static EventManager& instance() {
        static EventManager instance;
        return instance;
    }

    /**
     * @brief Registers a listener for a specific event type.
     * @param event_type The event type to listen for.
     * @param listener The listener to register.
     */
    void registerListener(EventType event_type, ListenerPtr listener) {
        event_bus_.registerListener(event_type, std::move(listener));
    }

    /**
     * @brief Unregisters a listener for a specific event type.
     * @param event_type The event type to unregister from.
     * @param listener The listener to unregister.
     */
    void unregisterListener(EventType event_type, const EventListener* listener) {
        event_bus_.unregisterListener(event_type, listener);
    }

    /**
     * @brief Pushes an event onto the internal event queue.
     * @param event The event to push.
     */
    void pushEvent(std::unique_ptr<Event> event) { event_queue_.push(std::move(event)); }

    /**
     * @brief Processes all events in the internal event queue.
     */
    void processEvents() { event_bus_.processEvents(event_queue_); }

    /**
     * @brief Dispatches a single event immediately (bypasses queue).
     * @param event The event to dispatch.
     */
    void dispatchImmediate(const Event& event) { event_bus_.dispatchImmediate(event); }

    /**
     * @brief Gets the number of pending events in the queue.
     * @return The number of pending events.
     */
    [[nodiscard]] size_t pendingEventCount() const { return event_queue_.size(); }

    /**
     * @brief Clears all pending events from the queue.
     */
    void clearPendingEvents() { event_queue_.clear(); }

   private:
    EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    EventBus event_bus_;
    EventQueue event_queue_;
};

}  // namespace vne::events
