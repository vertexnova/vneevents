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
    void handleMouseMove(double x, double y);
    void handleMouseButton(int button, int action, int mods);
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

    vne::events::EventManager& event_manager_;
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;
};

}  // namespace vne::events::examples
