#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Input polling demonstration helper class
 * ----------------------------------------------------------------------
 */

namespace vne::events::examples {

/**
 * @class InputDemo
 * @brief Helper class that demonstrates input polling usage.
 *
 * This class demonstrates:
 * - Polling keyboard state
 * - Polling mouse state
 * - Per-frame input tracking
 * - Input state management with nextFrame()
 */
class InputDemo {
   public:
    InputDemo() = default;
    ~InputDemo() = default;

    // Non-copyable, movable
    InputDemo(const InputDemo&) = delete;
    InputDemo& operator=(const InputDemo&) = delete;
    InputDemo(InputDemo&&) noexcept = default;
    InputDemo& operator=(InputDemo&&) noexcept = default;

    /**
     * @brief Runs the input polling demonstration.
     */
    void run();

   private:
    /**
     * @brief Demonstrates keyboard polling.
     */
    void demonstrateKeyboardPolling();

    /**
     * @brief Demonstrates mouse polling.
     */
    void demonstrateMousePolling();

    /**
     * @brief Demonstrates per-frame state management.
     */
    void demonstratePerFrameState();
};

}  // namespace vne::events::examples
