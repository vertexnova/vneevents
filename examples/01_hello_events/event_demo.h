#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event demonstration helper class for Hello Events example
 * ----------------------------------------------------------------------
 */

#include <vertexnova/events/events.h>

#include <memory>

namespace vne::events::examples {

/**
 * @class EventDemo
 * @brief Helper class that demonstrates basic event system usage.
 *
 * This class encapsulates the logic for demonstrating:
 * - Creating event listeners
 * - Registering listeners with EventManager
 * - Pushing and processing events
 * - Handling different event types
 */
class EventDemo {
   public:
    EventDemo() = default;
    ~EventDemo() = default;

    // Non-copyable, movable
    EventDemo(const EventDemo&) = delete;
    EventDemo& operator=(const EventDemo&) = delete;
    EventDemo(EventDemo&&) noexcept = default;
    EventDemo& operator=(EventDemo&&) noexcept = default;

    /**
     * @brief Runs the event demonstration.
     */
    void run();

   private:
    /**
     * @brief Demonstrates keyboard event handling.
     */
    void demonstrateKeyboardEvents();

    /**
     * @brief Demonstrates mouse event handling.
     */
    void demonstrateMouseEvents();

    /**
     * @brief Demonstrates window event handling.
     */
    void demonstrateWindowEvents();
};

}  // namespace vne::events::examples
