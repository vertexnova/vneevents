#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Multithreaded events demonstration helper class
 * ----------------------------------------------------------------------
 */

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

namespace vne::events::examples {

/**
 * @class ThreadedDemo
 * @brief Helper class that demonstrates thread-safe event processing.
 *
 * This class demonstrates:
 * - Events pushed from multiple threads
 * - Thread-safe event queue
 * - Concurrent listener registration
 * - No event corruption or race conditions
 */
class ThreadedDemo {
   public:
    ThreadedDemo() = default;
    ~ThreadedDemo() = default;

    // Non-copyable, non-movable (atomic is not movable)
    ThreadedDemo(const ThreadedDemo&) = delete;
    ThreadedDemo& operator=(const ThreadedDemo&) = delete;
    ThreadedDemo(ThreadedDemo&&) noexcept = delete;
    ThreadedDemo& operator=(ThreadedDemo&&) noexcept = delete;

    /**
     * @brief Runs the multithreaded events demonstration.
     */
    void run();

   private:
    /**
     * @brief Demonstrates concurrent event pushing from multiple threads.
     */
    void demonstrateConcurrentEventPushing();

    /**
     * @brief Demonstrates concurrent listener registration.
     */
    void demonstrateConcurrentListenerRegistration();

    /**
     * @brief Demonstrates thread-safe event processing.
     */
    void demonstrateThreadSafeProcessing();

    // Thread synchronization
    std::atomic<int> events_received_{0};
};

}  // namespace vne::events::examples
