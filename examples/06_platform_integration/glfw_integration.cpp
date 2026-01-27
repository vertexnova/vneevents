/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * GLFW integration implementation
 * ----------------------------------------------------------------------
 */

#include "glfw_integration.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

namespace {
    CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.platform_integration")
}

namespace vne::events::examples {

GLFWIntegration::GLFWIntegration(vne::events::EventManager& event_manager)
    : event_manager_(event_manager)
    , window_(nullptr)
    , initialized_(false) {}

GLFWIntegration::~GLFWIntegration() {
    shutdown();
}

bool GLFWIntegration::initialize(int width, int height, const char* title) {
    // Initialize GLFW
    if (!glfwInit()) {
        VNE_LOG_ERROR << "Failed to initialize GLFW";
        return false;
    }
    initialized_ = true;

    // Configure OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        VNE_LOG_ERROR << "Failed to create GLFW window";
        glfwTerminate();
        initialized_ = false;
        return false;
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(window_);
    
    // Initialize GLAD - must be called after context is current
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        VNE_LOG_ERROR << "Failed to initialize GLAD";
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
        initialized_ = false;
        return false;
    }
    
    // Log OpenGL information
    VNE_LOG_INFO << "  [GLAD] OpenGL version: " << glGetString(GL_VERSION);
    VNE_LOG_INFO << "  [GLAD] OpenGL vendor: " << glGetString(GL_VENDOR);
    VNE_LOG_INFO << "  [GLAD] OpenGL renderer: " << glGetString(GL_RENDERER);
    
    glfwSwapInterval(1);  // Enable VSync

    // Set window user pointer to this instance for callbacks
    glfwSetWindowUserPointer(window_, this);

    // Set GLFW callbacks
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, mouseMoveCallback);
    glfwSetWindowSizeCallback(window_, windowResizeCallback);
    glfwSetWindowCloseCallback(window_, windowCloseCallback);

    VNE_LOG_INFO << "  [GLFW] Window created: " << width << "x" << height;
    VNE_LOG_INFO << "  [GLFW] OpenGL 3.3 Core Profile context created";
    VNE_LOG_INFO << "  [GLFW] Callbacks registered";

    return true;
}

bool GLFWIntegration::shouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void GLFWIntegration::pollEvents() {
    if (window_) {
        glfwPollEvents();
    }
}

void GLFWIntegration::swapBuffers() {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

void GLFWIntegration::shutdown() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    if (initialized_) {
        glfwTerminate();
        initialized_ = false;
    }
}

void GLFWIntegration::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* integration = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(window));
    if (integration != nullptr) {
        integration->handleKeyEvent(key, scancode, action, mods);
    }
}

void GLFWIntegration::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* integration = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(window));
    if (integration != nullptr) {
        integration->handleMouseButton(button, action, mods);
    }
}

void GLFWIntegration::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* integration = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(window));
    if (integration != nullptr) {
        integration->handleMouseMove(xpos, ypos);
    }
}

void GLFWIntegration::windowResizeCallback(GLFWwindow* window, int width, int height) {
    auto* integration = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(window));
    if (integration != nullptr) {
        integration->handleWindowResize(width, height);
    }
}

void GLFWIntegration::windowCloseCallback(GLFWwindow* window) {
    auto* integration = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(window));
    if (integration != nullptr) {
        integration->handleWindowClose();
    }
}

void GLFWIntegration::handleKeyEvent(int key, int scancode, int action, int mods) {
    vne::events::KeyCode key_code = glfwKeyToKeyCode(key);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // Push event to EventManager
        event_manager_.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(key_code));

        // Update Input state for polling
        vne::events::Input::updateKeyState(static_cast<int>(key_code), true);

        VNE_LOG_INFO << "    [Callback] Key pressed: " << key << " -> KeyCode::" << static_cast<int>(key_code);

        // ESC key closes window
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }
    } else if (action == GLFW_RELEASE) {
        // Push event to EventManager
        event_manager_.pushEvent(std::make_unique<vne::events::KeyReleasedEvent>(key_code));

        // Update Input state for polling
        vne::events::Input::updateKeyState(static_cast<int>(key_code), false);

        VNE_LOG_INFO << "    [Callback] Key released: " << key << " -> KeyCode::" << static_cast<int>(key_code);
    }
}

void GLFWIntegration::handleMouseButton(int button, int action, int mods) {
    vne::events::MouseButton mouse_button = glfwButtonToMouseButton(button);

    if (action == GLFW_PRESS) {
        // Push event to EventManager
        event_manager_.pushEvent(std::make_unique<vne::events::MouseButtonPressedEvent>(mouse_button));

        // Update Input state for polling
        vne::events::Input::updateMouseButtonState(static_cast<int>(mouse_button), true);

        VNE_LOG_INFO << "    [Callback] Mouse button pressed: " << button << " -> MouseButton::" << static_cast<int>(mouse_button);
    } else if (action == GLFW_RELEASE) {
        // Push event to EventManager
        event_manager_.pushEvent(std::make_unique<vne::events::MouseButtonReleasedEvent>(mouse_button));

        // Update Input state for polling
        vne::events::Input::updateMouseButtonState(static_cast<int>(mouse_button), false);

        VNE_LOG_INFO << "    [Callback] Mouse button released: " << button << " -> MouseButton::" << static_cast<int>(mouse_button);
    }
}

void GLFWIntegration::handleMouseMove(double x, double y) {
    // Push event to EventManager
    event_manager_.pushEvent(std::make_unique<vne::events::MouseMovedEvent>(x, y));

    // Update Input state for polling
    vne::events::Input::updateMousePosition(static_cast<int>(x), static_cast<int>(y));
}

void GLFWIntegration::handleWindowResize(int width, int height) {
    // Push event to EventManager
    event_manager_.pushEvent(std::make_unique<vne::events::WindowResizeEvent>(width, height));

    // Update OpenGL viewport
    glViewport(0, 0, width, height);

    VNE_LOG_INFO << "    [Callback] Window resized: " << width << "x" << height;
}

void GLFWIntegration::handleWindowClose() {
    // Push event to EventManager
    event_manager_.pushEvent(std::make_unique<vne::events::WindowCloseEvent>());

    VNE_LOG_INFO << "    [Callback] Window close requested";
}


vne::events::KeyCode GLFWIntegration::glfwKeyToKeyCode(int glfw_key) {
    // Map common GLFW key codes to VneEvents KeyCode
    // GLFW key codes match ASCII for letters/numbers, and use special values for function keys
    // This is a simplified mapping - a full implementation would map all keys
    switch (glfw_key) {
        case GLFW_KEY_SPACE:
            return vne::events::KeyCode::eSpace;
        case GLFW_KEY_A:
            return vne::events::KeyCode::eA;
        case GLFW_KEY_D:
            return vne::events::KeyCode::eD;
        case GLFW_KEY_S:
            return vne::events::KeyCode::eS;
        case GLFW_KEY_W:
            return vne::events::KeyCode::eW;
        case GLFW_KEY_ESCAPE:
            return vne::events::KeyCode::eEscape;
        case GLFW_KEY_ENTER:
            return vne::events::KeyCode::eEnter;
        default:
            // For unmapped keys, return unknown
            // In a full implementation, you would map all GLFW keys
            return vne::events::KeyCode::eUnknown;
    }
}

vne::events::MouseButton GLFWIntegration::glfwButtonToMouseButton(int glfw_button) {
    // Map GLFW mouse button codes to VneEvents MouseButton
    switch (glfw_button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return vne::events::MouseButton::eLeft;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return vne::events::MouseButton::eRight;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return vne::events::MouseButton::eMiddle;
        default:
            return vne::events::MouseButton::eLeft;  // Default to left button
    }
}

}  // namespace vne::events::examples
