#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Static input interface for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "input_manager.h"

#include <utility>

namespace vne::events {

/**
 * @class Input
 * @brief Static interface for querying input state.
 *
 * Use this class to poll input state anywhere in the engine or game code.
 * All functions are static and thread-safe.
 *
 * @code
 * if (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eA))) {
 *     // Handle 'A' key pressed
 * }
 * @endcode
 *
 * @threadsafe All methods are thread-safe.
 */
class VNEEVENTS_API Input{public :
                              // Keyboard
                              [[nodiscard]] static bool isKeyPressed(int key){return InputManager::isKeyPressed(key);
}  // namespace vne::events

[[nodiscard]] static bool isKeyJustPressed(int key) {
    return InputManager::isKeyJustPressed(key);
}

[[nodiscard]] static bool isKeyJustReleased(int key) {
    return InputManager::isKeyJustReleased(key);
}

// Mouse buttons
[[nodiscard]] static bool isMouseButtonPressed(int button) {
    return InputManager::isMouseButtonPressed(button);
}

[[nodiscard]] static bool isMouseButtonJustPressed(int button) {
    return InputManager::isMouseButtonJustPressed(button);
}

[[nodiscard]] static bool isMouseButtonJustReleased(int button) {
    return InputManager::isMouseButtonJustReleased(button);
}

// Mouse position and scroll
[[nodiscard]] static std::pair<int, int> mousePosition() {
    return InputManager::mousePosition();
}

[[nodiscard]] static std::pair<float, float> mouseScroll() {
    return InputManager::mouseScroll();
}

// Window size
[[nodiscard]] static std::pair<int, int> windowSize() {
    return InputManager::windowSize();
}

// Per-frame reset
static void nextFrame() {
    InputManager::nextFrame();
}

// Gamepad (stub)
[[nodiscard]] static bool isGamepadButtonPressed(int gamepad, int button) {
    return InputManager::isGamepadButtonPressed(gamepad, button);
}

// Update methods for platform integration
static void updateKeyState(int key, bool pressed) {
    InputManager::instance().inputState().updateKeyState(key, pressed);
}

static void updateMouseButtonState(int button, bool pressed) {
    InputManager::instance().inputState().updateMouseButtonState(button, pressed);
}

static void updateMousePosition(int x, int y) {
    InputManager::instance().inputState().updateMousePosition(x, y);
}

static void updateMouseScroll(float x_offset, float y_offset) {
    InputManager::instance().inputState().updateMouseScroll(x_offset, y_offset);
}

static void updateWindowSize(int width, int height) {
    InputManager::instance().inputState().updateWindowSize(width, height);
}
}
;

}  // namespace vne::events
