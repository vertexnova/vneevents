#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener classes for Multithreaded Events example
 * ----------------------------------------------------------------------
 */

#include <vertexnova/events/events.h>

#include <atomic>

namespace vne::events::examples {

/**
 * @class EventCounter
 * @brief Listener that counts events for testing thread safety.
 */
class EventCounter : public vne::events::EventListener {
   public:
    explicit EventCounter(std::atomic<int>* counter);
    ~EventCounter() override = default;

    void onEvent(const vne::events::Event& event) override;

   private:
    std::atomic<int>* counter_;
};

/**
 * @class ThreadListener
 * @brief Listener that identifies which thread received an event.
 */
class ThreadListener : public vne::events::EventListener {
   public:
    explicit ThreadListener(int thread_id);
    ~ThreadListener() override = default;

    void onEvent(const vne::events::Event& event) override;

   private:
    int thread_id_;
};

/**
 * @class SafeListener
 * @brief Listener for testing thread-safe event processing.
 */
class SafeListener : public vne::events::EventListener {
   public:
    explicit SafeListener(std::atomic<int>* counter);
    ~SafeListener() override = default;

    void onEvent(const vne::events::Event& event) override;

   private:
    std::atomic<int>* counter_;
};

}  // namespace vne::events::examples
