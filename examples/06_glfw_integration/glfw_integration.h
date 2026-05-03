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
#include <glad/glad.h>

#include <vertexnova/events/events.h>
#include <array>
#include <memory>

namespace vne::events::examples {

class GLFWIntegration {
   public:
    explicit GLFWIntegration(vne::events::EventManager& event_manager);
    ~GLFWIntegration();

    GLFWIntegration(const GLFWIntegration&) = delete;
    GLFWIntegration& operator=(const GLFWIntegration&) = delete;
    GLFWIntegration(GLFWIntegration&&) noexcept = delete;
    GLFWIntegration& operator=(GLFWIntegration&&) noexcept = delete;

    bool initialize(int width, int height, const char* title);
    void shutdown();
    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    GLFWwindow* getWindow() const { return window_; }

    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleMouseMove(GLFWwindow* window, double x, double y);
    void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
    void handleWindowResize(int width, int height);
    void handleWindowClose();

   private:
    static vne::events::KeyCode glfwKeyToKeyCode(int glfw_key);
    static vne::events::MouseButton glfwButtonToMouseButton(int glfw_button);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double x, double y);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void windowResizeCallback(GLFWwindow* window, int width, int height);
    static void windowCloseCallback(GLFWwindow* window);

    static constexpr double kDoubleClickMaxIntervalSeconds_ = 0.3;  // 300 ms
    static constexpr double kDoubleClickMaxDistancePixels_ = 4.0;   // small movement
    /** Sentinel for last_click_time_ meaning "no previous click"; first click never counts as double. */
    static const double kNoClickTimeSeconds_;

    vne::events::EventManager& event_manager_;
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;

    // Per-button state for double-click detection (indexed by GLFW mouse button)
    std::array<double, GLFW_MOUSE_BUTTON_LAST + 1> last_click_time_{};  // glfwGetTime() or kNoClickTimeSeconds_
    std::array<double, GLFW_MOUSE_BUTTON_LAST + 1> last_click_x_{};
    std::array<double, GLFW_MOUSE_BUTTON_LAST + 1> last_click_y_{};
};

}  // namespace vne::events::examples
