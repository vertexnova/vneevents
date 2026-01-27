/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Platform integration demonstration implementation
 * ----------------------------------------------------------------------
 */

#include "platform_demo.h"

#include "glfw_integration.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

// GLAD is included via glfw_integration.h

#include <thread>

namespace {
    CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.platform_integration")
}

namespace vne::events::examples {

void PlatformDemo::run() {
    VNE_LOG_INFO << "=== Platform Integration Demonstration ===";
    VNE_LOG_INFO << "";

    demonstrateGLFWIntegration();
    demonstrateEventTranslation();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demonstration Complete ===";
}

void PlatformDemo::demonstrateGLFWIntegration() {
    VNE_LOG_INFO << "--- GLFW Integration ---";

    auto& manager = vne::events::EventManager::instance();
    auto glfw_integration = std::make_unique<GLFWIntegration>(manager);

    // Initialize GLFW and create window with OpenGL 3.3
    if (!glfw_integration->initialize(800, 600, "VneEvents Platform Integration Example")) {
        VNE_LOG_ERROR << "Failed to initialize GLFW window";
        VNE_LOG_INFO << "  (This example requires a display/window system)";
        VNE_LOG_INFO << "  Skipping interactive demonstration...";
        return;
    }

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "  Window is now open. Try:";
    VNE_LOG_INFO << "    - Press keys (W, A, S, D, Space, ESC)";
    VNE_LOG_INFO << "    - Move mouse";
    VNE_LOG_INFO << "    - Click mouse buttons";
    VNE_LOG_INFO << "    - Resize window";
    VNE_LOG_INFO << "    - Press ESC to close";
    VNE_LOG_INFO << "";

    // Run for a short time or until window closes
    // In a real application, this would run until the user closes the window
    int frame_count = 0;
    const int max_frames = 300;  // ~5 seconds at 60 FPS

    // Simple render function
    auto render = [&glfw_integration]() {
        // Simple OpenGL rendering - clear with a color
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    while (!glfw_integration->shouldClose() && frame_count < max_frames) {
        // Poll events (triggers GLFW callbacks which push to EventManager)
        glfw_integration->pollEvents();

        // Process events (from GLFW callbacks)
        manager.processEvents();

        // Render
        render();

        // Swap buffers
        glfw_integration->swapBuffers();

        // Reset per-frame input state
        vne::events::Input::nextFrame();

        frame_count++;
    }

    if (frame_count >= max_frames) {
        VNE_LOG_INFO << "  Demo time limit reached, closing window...";
    }

    glfw_integration->shutdown();

    VNE_LOG_INFO << "";
}

void PlatformDemo::demonstrateEventTranslation() {
    VNE_LOG_INFO << "--- Event Translation ---";

    VNE_LOG_INFO << "  Event translation is demonstrated in the GLFW callbacks:";
    VNE_LOG_INFO << "    - GLFW_KEY_W -> KeyPressedEvent(KeyCode::eW)";
    VNE_LOG_INFO << "    - GLFW_MOUSE_BUTTON_LEFT -> MouseButtonPressedEvent(MouseButton::eLeft)";
    VNE_LOG_INFO << "    - GLFW window resize -> WindowResizeEvent(width, height)";
    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "  All events are automatically translated when you interact with the window.";
    VNE_LOG_INFO << "";
}

}  // namespace vne::events::examples
