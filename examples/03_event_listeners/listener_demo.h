#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Multiple event listeners demonstration helper class
 * ----------------------------------------------------------------------
 */

#include <memory>

namespace vne::events::examples {

/**
 * @class ListenerDemo
 * @brief Helper class that demonstrates multiple event listeners.
 *
 * This class demonstrates:
 * - Multiple listeners per event type
 * - Separate listeners for different subsystems
 * - Event filtering and handling patterns
 * - Listener lifecycle management
 */
class ListenerDemo {
   public:
    ListenerDemo() = default;
    ~ListenerDemo() = default;

    // Non-copyable, movable
    ListenerDemo(const ListenerDemo&) = delete;
    ListenerDemo& operator=(const ListenerDemo&) = delete;
    ListenerDemo(ListenerDemo&&) noexcept = default;
    ListenerDemo& operator=(ListenerDemo&&) noexcept = default;

    /**
     * @brief Runs the multiple listeners demonstration.
     */
    void run();

   private:
    /**
     * @brief Demonstrates multiple listeners for the same event type.
     */
    void demonstrateMultipleListeners();

    /**
     * @brief Demonstrates separate listeners for different subsystems.
     */
    void demonstrateSubsystemListeners();

    /**
     * @brief Demonstrates listener registration and unregistration.
     */
    void demonstrateListenerLifecycle();
};

}  // namespace vne::events::examples
