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
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.glfw_integration");


uint8_t glfwModsToModifierKey(int mods) {
    uint8_t out = 0;
    if (mods & GLFW_MOD_SHIFT) {
        out |= static_cast<uint8_t>(vne::events::ModifierKey::eModShift);
    }
    if (mods & GLFW_MOD_CONTROL) {
        out |= static_cast<uint8_t>(vne::events::ModifierKey::eModCtrl);
    }
    if (mods & GLFW_MOD_ALT) {
        out |= static_cast<uint8_t>(vne::events::ModifierKey::eModAlt);
    }
    if (mods & GLFW_MOD_SUPER) {
        out |= static_cast<uint8_t>(vne::events::ModifierKey::eModSuper);
    }
    return out;
}

uint8_t glfwQueryModifiers(GLFWwindow* w) {
    int mods = 0;
    if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
        || glfwGetKey(w, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        mods |= GLFW_MOD_SHIFT;
    }
    if (glfwGetKey(w, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
        || glfwGetKey(w, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        mods |= GLFW_MOD_CONTROL;
    }
    if (glfwGetKey(w, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
        || glfwGetKey(w, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
        mods |= GLFW_MOD_ALT;
    }
    if (glfwGetKey(w, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS
        || glfwGetKey(w, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS) {
        mods |= GLFW_MOD_SUPER;
    }
    return glfwModsToModifierKey(mods);
}

} // namespace

namespace vne::events::examples {

GLFWIntegration::GLFWIntegration(vne::events::EventManager& event_manager)
    : event_manager_(event_manager)
    , window_(nullptr)
    , initialized_(false) {
    // Initialize double-click tracking state (kNoClickTime = no previous click; first click never double)
    last_click_time_.fill(kNoClickTime);
    last_click_x_.fill(0.0);
    last_click_y_.fill(0.0);
}

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

void GLFWIntegration::keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr) {
        i->handleKeyEvent(key, scancode, action, mods);
    }
}

void GLFWIntegration::mouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr) {
        i->handleMouseButton(w, button, action, mods);
    }
}

void GLFWIntegration::mouseMoveCallback(GLFWwindow* w, double xpos, double ypos) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr) {
        i->handleMouseMove(w, xpos, ypos);
    }
}

void GLFWIntegration::windowResizeCallback(GLFWwindow* w, int width, int height) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr) {
        i->handleWindowResize(width, height);
    }
}

void GLFWIntegration::windowCloseCallback(GLFWwindow* w) {
    auto* i = reinterpret_cast<GLFWIntegration*>(glfwGetWindowUserPointer(w));
    if (i != nullptr) {
        i->handleWindowClose();
    }
}

void GLFWIntegration::handleKeyEvent(int key, int /* scancode */, int action, int mods) {
    vne::events::KeyCode k = glfwKeyToKeyCode(key);
    const uint8_t m = glfwModsToModifierKey(mods);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        event_manager_.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(k, m));
        vne::events::Input::updateKeyState(static_cast<int>(k), true);
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
    } else if (action == GLFW_RELEASE) {
        event_manager_.pushEvent(std::make_unique<vne::events::KeyReleasedEvent>(k, m));
        vne::events::Input::updateKeyState(static_cast<int>(k), false);
    }
}

void GLFWIntegration::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    double x = 0, y = 0;
    glfwGetCursorPos(window, &x, &y);
    vne::events::MouseButton b = glfwButtonToMouseButton(button);
    const uint8_t m = glfwModsToModifierKey(mods);

    bool is_double_click = false;
    const int idx = button;
    if (action == GLFW_PRESS && idx >= 0 && idx <= GLFW_MOUSE_BUTTON_LAST) {
        const double now = glfwGetTime();
        const std::size_t i = static_cast<std::size_t>(idx);
        // Only attempt double-click detection if we have a valid previous click time.
        if (last_click_time_[i] != kNoClickTime) {
            const double dt = now - last_click_time_[i];
            const double dx = x - last_click_x_[i];
            const double dy = y - last_click_y_[i];
            const double dist2 = dx * dx + dy * dy;
            if (dt > 0.0 && dt <= kDoubleClickMaxIntervalSeconds_ &&
                dist2 <= kDoubleClickMaxDistancePixels_ * kDoubleClickMaxDistancePixels_) {
                is_double_click = true;
            }
        }
        last_click_time_[i] = now;
        last_click_x_[i] = x;
        last_click_y_[i] = y;
    }

    if (action == GLFW_PRESS) {
        event_manager_.pushEvent(std::make_unique<vne::events::MouseButtonPressedEvent>(b, m, x, y));
        vne::events::Input::updateMouseButtonState(static_cast<int>(b), true);
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            event_manager_.pushEvent(std::make_unique<vne::events::TouchPressEvent>(0, x, y));
        }
        if (is_double_click) {
            event_manager_.pushEvent(
                std::make_unique<vne::events::MouseButtonDoubleClickedEvent>(b, m, x, y));
        }
    } else if (action == GLFW_RELEASE) {
        event_manager_.pushEvent(std::make_unique<vne::events::MouseButtonReleasedEvent>(b, m, x, y));
        vne::events::Input::updateMouseButtonState(static_cast<int>(b), false);
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            event_manager_.pushEvent(std::make_unique<vne::events::TouchReleaseEvent>(0, x, y));
        }
    }
}

void GLFWIntegration::handleMouseMove(GLFWwindow* window, double x, double y) {
    const uint8_t m = glfwQueryModifiers(window);
    event_manager_.pushEvent(std::make_unique<vne::events::MouseMovedEvent>(x, y, m));
    vne::events::Input::updateMousePosition(static_cast<int>(x), static_cast<int>(y));
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        event_manager_.pushEvent(std::make_unique<vne::events::TouchMoveEvent>(0, x, y));
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
