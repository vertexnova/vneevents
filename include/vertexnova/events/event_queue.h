#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Thread-safe event queue for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "internal/read_write_mutex.h"

#include <queue>
#include <memory>

namespace vne::events {

/**
 * @class EventQueue
 * @brief Thread-safe queue for storing events.
 *
 * This class manages a queue of unique pointers to Event objects,
 * providing thread-safe push/pop operations.
 *
 * @threadsafe All public methods are thread-safe.
 */
class EventQueue {
   public:
    using EventPtr = std::unique_ptr<Event>;

    EventQueue() = default;
    ~EventQueue() = default;

    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    /**
     * @brief Push an event onto the queue.
     * @param event The event to push.
     */
    void push(EventPtr event) {
        internal::WriteLockGuard lock(mutex_);
        queue_.push(std::move(event));
    }

    /**
     * @brief Pop an event from the front of the queue.
     * @return The event, or nullptr if the queue is empty.
     */
    [[nodiscard]] EventPtr pop() {
        internal::WriteLockGuard lock(mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        EventPtr event = std::move(queue_.front());
        queue_.pop();
        return event;
    }

    /**
     * @brief Check if the queue is empty.
     * @return True if the queue is empty.
     */
    [[nodiscard]] bool empty() const {
        internal::ReadLockGuard lock(mutex_);
        return queue_.empty();
    }

    /**
     * @brief Get the number of events in the queue.
     * @return The number of events.
     */
    [[nodiscard]] size_t size() const {
        internal::ReadLockGuard lock(mutex_);
        return queue_.size();
    }

    /**
     * @brief Clear all events from the queue.
     */
    void clear() {
        internal::WriteLockGuard lock(mutex_);
        while (!queue_.empty()) {
            queue_.pop();
        }
    }

   private:
    std::queue<EventPtr> queue_;
    mutable internal::ReadWriteMutex mutex_;
};

}  // namespace vne::events
