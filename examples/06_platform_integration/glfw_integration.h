#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * GLFW integration helper for translating GLFW events to VneEvents
 * ----------------------------------------------------------------------
 */

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Include GLAD before any OpenGL calls
#include <glad/glad.h>

#include <vertexnova/events/events.h>

#include <memory>

namespace vne::events::examples {

/**
 * @class GLFWIntegration
 * @brief Helper class for integrating GLFW callbacks with VneEvents.
 *
 * This class manages a GLFW window with OpenGL 3.3 context and translates
 * GLFW events into VneEvents. It demonstrates:
 * 1. GLFW window creation and OpenGL context setup
 * 2. GLFW callback registration
 * 3. Event translation from GLFW to VneEvents
 * 4. Input state updates from GLFW callbacks
 * 5. Game loop with event processing and rendering
 */
class GLFWIntegration {
   public:
    explicit GLFWIntegration(vne::events::EventManager& event_manager);
    ~GLFWIntegration();

    // Non-copyable, non-movable
    GLFWIntegration(const GLFWIntegration&) = delete;
    GLFWIntegration& operator=(const GLFWIntegration&) = delete;
    GLFWIntegration(GLFWIntegration&&) noexcept = delete;
    GLFWIntegration& operator=(GLFWIntegration&&) noexcept = delete;

    /**
     * @brief Initializes GLFW and creates a window with OpenGL 3.3 context.
     * @param width Window width
     * @param height Window height
     * @param title Window title
     * @return True if successful, false otherwise
     */
    bool initialize(int width, int height, const char* title);

    /**
     * @brief Cleans up GLFW resources.
     */
    void shutdown();

    /**
     * @brief Checks if the window should close.
     * @return True if window should close
     */
    bool shouldClose() const;

    /**
     * @brief Polls for events and swaps buffers.
     */
    void pollEvents();

    /**
     * @brief Swaps the front and back buffers.
     */
    void swapBuffers();

    /**
     * @brief Gets the GLFW window handle.
     * @return GLFW window pointer
     */
    GLFWwindow* getWindow() const { return window_; }

    /**
     * @brief Handles a GLFW key event (called from callback).
     * @param key GLFW key code
     * @param scancode System-specific scancode
     * @param action GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT
     * @param mods Modifier keys
     */
    void handleKeyEvent(int key, int scancode, int action, int mods);

    /**
     * @brief Handles a GLFW mouse move event (called from callback).
     * @param x Mouse X position
     * @param y Mouse Y position
     */
    void handleMouseMove(double x, double y);

    /**
     * @brief Handles a GLFW mouse button event (called from callback).
     * @param button GLFW button code
     * @param action GLFW_PRESS or GLFW_RELEASE
     * @param mods Modifier keys
     */
    void handleMouseButton(int button, int action, int mods);

    /**
     * @brief Handles a GLFW window resize event (called from callback).
     * @param width New window width
     * @param height New window height
     */
    void handleWindowResize(int width, int height);

    /**
     * @brief Handles a GLFW window close event (called from callback).
     */
    void handleWindowClose();

   private:
    /**
     * @brief Converts GLFW key code to VneEvents KeyCode.
     * @param glfw_key GLFW key code
     * @return VneEvents KeyCode
     */
    static vne::events::KeyCode glfwKeyToKeyCode(int glfw_key);

    /**
     * @brief Converts GLFW mouse button to VneEvents MouseButton.
     * @param glfw_button GLFW button code
     * @return VneEvents MouseButton
     */
    static vne::events::MouseButton glfwButtonToMouseButton(int glfw_button);

    /**
     * @brief Static callback wrappers for GLFW (C-style callbacks).
     */
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double x, double y);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void windowResizeCallback(GLFWwindow* window, int width, int height);
    static void windowCloseCallback(GLFWwindow* window);

    vne::events::EventManager& event_manager_;
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;
};

}  // namespace vne::events::examples
