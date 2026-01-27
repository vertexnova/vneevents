#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Platform integration demonstration helper class
 * ----------------------------------------------------------------------
 */

namespace vne::events::examples {

/**
 * @class PlatformDemo
 * @brief Helper class that demonstrates GLFW platform integration.
 *
 * This class demonstrates:
 * - GLFW callback integration
 * - Platform event translation
 * - Input state updates from callbacks
 * - Cross-platform abstraction
 */
class PlatformDemo {
   public:
    PlatformDemo() = default;
    ~PlatformDemo() = default;

    // Non-copyable, non-movable
    PlatformDemo(const PlatformDemo&) = delete;
    PlatformDemo& operator=(const PlatformDemo&) = delete;
    PlatformDemo(PlatformDemo&&) noexcept = delete;
    PlatformDemo& operator=(PlatformDemo&&) noexcept = delete;

    /**
     * @brief Runs the platform integration demonstration.
     */
    void run();

   private:
    /**
     * @brief Demonstrates GLFW window creation and event callbacks.
     */
    void demonstrateGLFWIntegration();

    /**
     * @brief Demonstrates event translation from GLFW to VneEvents.
     */
    void demonstrateEventTranslation();
};

}  // namespace vne::events::examples
