/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Multithreaded events demonstration implementation
 * ----------------------------------------------------------------------
 */

#include "threaded_demo.h"

#include "listeners.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

#include <thread>
#include <vector>

namespace {
    CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.multithreaded_events")
}

namespace vne::events::examples {

void ThreadedDemo::run() {
    VNE_LOG_INFO << "=== Multithreaded Events Demonstration ===";
    VNE_LOG_INFO << "";

    auto& manager = vne::events::EventManager::instance();

    demonstrateConcurrentEventPushing();
    
    // Clear any remaining events and reset state
    manager.clearPendingEvents();
    events_received_ = 0;

    demonstrateConcurrentListenerRegistration();
    
    // Clear any remaining events and reset state
    manager.clearPendingEvents();
    events_received_ = 0;

    demonstrateThreadSafeProcessing();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demonstration Complete ===";
}

void ThreadedDemo::demonstrateConcurrentEventPushing() {
    VNE_LOG_INFO << "--- Concurrent Event Pushing ---";

    auto& manager = vne::events::EventManager::instance();

    auto counter = std::make_shared<EventCounter>(&events_received_);
    manager.registerListener(vne::events::EventType::eKeyPressed, counter);

    const int num_threads = 4;
    const int events_per_thread = 10;

    VNE_LOG_INFO << "  Starting " << num_threads << " threads";
    VNE_LOG_INFO << "  Each thread will push " << events_per_thread << " events";
    VNE_LOG_INFO << "  Total events expected: " << (num_threads * events_per_thread);

    std::vector<std::thread> threads;

    // Launch threads that push events concurrently
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&manager, i]() {
            for (int j = 0; j < events_per_thread; ++j) {
                // Each thread pushes events with different key codes
                int key_code = static_cast<int>(vne::events::KeyCode::eA) + (i * events_per_thread) + j;
                manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(
                    static_cast<vne::events::KeyCode>(key_code)));
            }
        });
    }

    // Wait for all threads to finish pushing
    for (auto& thread : threads) {
        thread.join();
    }

    VNE_LOG_INFO << "  All threads completed pushing events";
    VNE_LOG_INFO << "  Processing events...";

    // Process all events
    manager.processEvents();

    VNE_LOG_INFO << "  Events received: " << events_received_.load();
    VNE_LOG_INFO << "  Expected: " << (num_threads * events_per_thread);
    VNE_LOG_INFO << "  Thread-safe: " << (events_received_.load() == num_threads * events_per_thread ? "✓ Yes" : "✗ No");

    // Unregister listener to avoid interference with next test
    manager.unregisterListener(vne::events::EventType::eKeyPressed, counter.get());
    events_received_ = 0;
    VNE_LOG_INFO << "";
}

void ThreadedDemo::demonstrateConcurrentListenerRegistration() {
    VNE_LOG_INFO << "--- Concurrent Listener Registration ---";

    auto& manager = vne::events::EventManager::instance();

    const int num_threads = 3;
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<vne::events::EventListener>> listeners;

    VNE_LOG_INFO << "  Registering " << num_threads << " listeners from different threads";

    // Launch threads that register listeners concurrently
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&manager, &listeners, i]() {
            auto listener = std::make_shared<ThreadListener>(i);
            listeners.push_back(listener);
            manager.registerListener(vne::events::EventType::eKeyPressed, listener);
        });
    }

    // Wait for all threads to finish registration
    for (auto& thread : threads) {
        thread.join();
    }

    VNE_LOG_INFO << "  All listeners registered";
    VNE_LOG_INFO << "  Pushing test event...";

    // Push a single event - all listeners should receive it
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eEnter));
    manager.processEvents();

    // Unregister listeners to avoid interference with next test
    for (const auto& listener : listeners) {
        manager.unregisterListener(vne::events::EventType::eKeyPressed, listener.get());
    }

    VNE_LOG_INFO << "";
}

void ThreadedDemo::demonstrateThreadSafeProcessing() {
    VNE_LOG_INFO << "--- Thread-Safe Event Processing ---";

    auto& manager = vne::events::EventManager::instance();

    auto safe_listener = std::make_shared<SafeListener>(&events_received_);
    manager.registerListener(vne::events::EventType::eMouseMoved, safe_listener);

    const int num_threads = 5;
    const int events_per_thread = 20;

    VNE_LOG_INFO << "  Testing thread-safe event queue";
    VNE_LOG_INFO << "  " << num_threads << " threads pushing events concurrently";
    VNE_LOG_INFO << "  Processing events after all threads complete";

    std::vector<std::thread> threads;
    std::atomic<bool> start_flag{false};
    std::atomic<int> threads_completed{0};

    // Launch producer threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&manager, &start_flag, &threads_completed, i]() {
            // Wait for start signal
            while (!start_flag.load()) {
                std::this_thread::yield();
            }

            // Push events
            for (int j = 0; j < events_per_thread; ++j) {
                double x = static_cast<double>(i * events_per_thread + j);
                double y = static_cast<double>(j);
                manager.pushEvent(std::make_unique<vne::events::MouseMovedEvent>(x, y));
            }

            threads_completed++;
        });
    }

    // Start all threads
    start_flag = true;

    // Wait for all threads to complete pushing
    for (auto& thread : threads) {
        thread.join();
    }

    VNE_LOG_INFO << "  All threads completed pushing events";
    
    // Verify queue size before processing
    size_t queue_size = manager.pendingEventCount();
    VNE_LOG_INFO << "  Events in queue: " << queue_size;
    VNE_LOG_INFO << "  Expected in queue: " << (num_threads * events_per_thread);
    
    VNE_LOG_INFO << "  Processing all events...";

    // Process all events after threads have finished
    manager.processEvents();
    VNE_LOG_INFO << "  Events received: " << events_received_.load();
    VNE_LOG_INFO << "  Expected: " << (num_threads * events_per_thread);
    VNE_LOG_INFO << "  Thread-safe: " << (events_received_.load() == num_threads * events_per_thread ? "✓ Yes" : "✗ No");

    events_received_ = 0;
    VNE_LOG_INFO << "";
}

}  // namespace vne::events::examples
