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
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.glfw_integration")
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
    if (!glfwInit()) {
        VNE_LOG_ERROR << "Failed to initialize GLFW";
        return false;
    }
    initialized_ = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        VNE_LOG_ERROR << "Failed to create GLFW window";
        glfwTerminate();
        initialized_ = false;
        return false;
    }

    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        VNE_LOG_ERROR << "Failed to initialize GLAD";
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
        initialized_ = false;
        return false;
    }

    VNE_LOG_INFO << "  [GLFW] Window created: " << width << "x" << height;
    VNE_LOG_INFO << "  [GLAD] OpenGL: " << glGetString(GL_VERSION);

    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window_, this);
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, mouseMoveCallback);
    glfwSetWindowSizeCallback(window_, windowResizeCallback);
    glfwSetWindowCloseCallback(window_, windowCloseCallback);

    return true;
}

bool GLFWIntegration::shouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void GLFWIntegration::pollEvents() {
    if (window_)
        glfwPollEvents();
}

void GLFWIntegration::swapBuffers() {
    if (window_)
        glfwSwapBuffers(window_);
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

void GLFWIntegration::keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr)
        i->handleKeyEvent(key, scancode, action, mods);
}

void GLFWIntegration::mouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr)
        i->handleMouseButton(button, action, mods);
}

void GLFWIntegration::mouseMoveCallback(GLFWwindow* w, double xpos, double ypos) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr)
        i->handleMouseMove(xpos, ypos);
}

void GLFWIntegration::windowResizeCallback(GLFWwindow* w, int width, int height) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr)
        i->handleWindowResize(width, height);
}

void GLFWIntegration::windowCloseCallback(GLFWwindow* w) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr)
        i->handleWindowClose();
}

void GLFWIntegration::handleKeyEvent(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    vne::events::KeyCode k = glfwKeyToKeyCode(key);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        event_manager_.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(k));
        vne::events::Input::updateKeyState(static_cast<int>(k), true);
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
    } else if (action == GLFW_RELEASE) {
        event_manager_.pushEvent(std::make_unique<vne::events::KeyReleasedEvent>(k));
        vne::events::Input::updateKeyState(static_cast<int>(k), false);
    }
}

void GLFWIntegration::handleMouseButton(int button, int action, [[maybe_unused]] int mods) {
    vne::events::MouseButton b = glfwButtonToMouseButton(button);
    if (action == GLFW_PRESS) {
        event_manager_.pushEvent(std::make_unique<vne::events::MouseButtonPressedEvent>(b));
        vne::events::Input::updateMouseButtonState(static_cast<int>(b), true);
    } else if (action == GLFW_RELEASE) {
        event_manager_.pushEvent(std::make_unique<vne::events::MouseButtonReleasedEvent>(b));
        vne::events::Input::updateMouseButtonState(static_cast<int>(b), false);
    }
}

void GLFWIntegration::handleMouseMove(double x, double y) {
    event_manager_.pushEvent(std::make_unique<vne::events::MouseMovedEvent>(x, y));
    vne::events::Input::updateMousePosition(static_cast<int>(x), static_cast<int>(y));
}

void GLFWIntegration::handleWindowResize(int width, int height) {
    event_manager_.pushEvent(std::make_unique<vne::events::WindowResizeEvent>(width, height));
    glViewport(0, 0, width, height);
}

void GLFWIntegration::handleWindowClose() {
    event_manager_.pushEvent(std::make_unique<vne::events::WindowCloseEvent>());
}

vne::events::KeyCode GLFWIntegration::glfwKeyToKeyCode(int glfw_key) {
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
            return vne::events::KeyCode::eUnknown;
    }
}

vne::events::MouseButton GLFWIntegration::glfwButtonToMouseButton(int glfw_button) {
    switch (glfw_button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return vne::events::MouseButton::eLeft;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return vne::events::MouseButton::eRight;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return vne::events::MouseButton::eMiddle;
        default:
            return vne::events::MouseButton::eLeft;
    }
}

}  // namespace vne::events::examples
