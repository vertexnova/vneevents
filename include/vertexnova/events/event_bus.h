#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event bus for VertexNova Events - combines queue and dispatcher.
 * ----------------------------------------------------------------------
 */

#include "event_dispatcher.h"
#include "event_queue.h"

#include <memory>

namespace vne::events {

/**
 * @class EventBus
 * @brief Manages event registration, queuing, and dispatching.
 *
 * The EventBus combines an event dispatcher with queue processing.
 * It allows registering listeners and processes events from a queue.
 *
 * @threadsafe All public methods are thread-safe.
 */
class EventBus {
   public:
    using ListenerPtr = std::shared_ptr<EventListener>;

    EventBus() = default;
    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    /**
     * @brief Registers a listener for a specific event type.
     * @param event_type The event type to listen for.
     * @param listener The listener to register.
     */
    void registerListener(EventType event_type, ListenerPtr listener) {
        dispatcher_.registerListener(event_type, std::move(listener));
    }

    /**
     * @brief Unregisters a listener for a specific event type.
     * @param event_type The event type to unregister from.
     * @param listener The listener to unregister.
     */
    void unregisterListener(EventType event_type, const EventListener* listener) {
        dispatcher_.unregisterListener(event_type, listener);
    }

    /**
     * @brief Processes all events in the provided event queue.
     * @param event_queue The queue to process events from.
     *
     * This function processes all events in the given queue, dispatching
     * each event to the registered listeners.
     *
     * @note This function processes events until the queue is empty.
     *       If events are added concurrently while processing, they will
     *       be processed in subsequent iterations.
     */
    void processEvents(EventQueue& event_queue) {
        // Process events until queue is empty
        // Use pop() which returns nullptr when empty, avoiding race condition
        // between empty() check and pop()
        while (true) {
            auto event = event_queue.pop();
            if (!event) {
                break;  // Queue is empty
            }
            dispatcher_.dispatch(*event);
        }
    }

    /**
     * @brief Dispatches a single event immediately (bypasses queue).
     * @param event The event to dispatch.
     */
    void dispatchImmediate(const Event& event) { dispatcher_.dispatch(event); }

    /**
     * @brief Gets the number of listeners for a specific event type.
     * @param event_type The event type to query.
     * @return The number of registered listeners.
     */
    [[nodiscard]] size_t listenerCount(EventType event_type) const { return dispatcher_.listenerCount(event_type); }

   private:
    EventDispatcher dispatcher_;
};

}  // namespace vne::events
