/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 * ----------------------------------------------------------------------
 */

#include "vertexnova/events/input/input_manager.h"

namespace vne::events {

InputManager::InputManager() = default;

InputManager& InputManager::instance() {
    static InputManager instance;
    return instance;
}

bool InputManager::isKeyPressed(int key) {
    return instance().input_state_.isKeyPressed(key);
}

bool InputManager::isKeyJustPressed(int key) {
    return instance().input_state_.isKeyJustPressed(key);
}

bool InputManager::isKeyJustReleased(int key) {
    return instance().input_state_.isKeyJustReleased(key);
}

bool InputManager::isMouseButtonPressed(int button) {
    return instance().input_state_.isMouseButtonPressed(button);
}

bool InputManager::isMouseButtonJustPressed(int button) {
    return instance().input_state_.isMouseButtonJustPressed(button);
}

bool InputManager::isMouseButtonJustReleased(int button) {
    return instance().input_state_.isMouseButtonJustReleased(button);
}

std::pair<int, int> InputManager::mousePosition() {
    return instance().input_state_.mousePosition();
}

std::pair<float, float> InputManager::mouseScroll() {
    return instance().input_state_.mouseScroll();
}

std::pair<int, int> InputManager::windowSize() {
    return instance().input_state_.windowSize();
}

void InputManager::nextFrame() {
    instance().input_state_.nextFrame();
}

bool InputManager::isGamepadButtonPressed(int /*gamepad*/, int /*button*/) {
    // Stub for future gamepad support
    return false;
}

const InputState& InputManager::inputState() const {
    return input_state_;
}

InputState& InputManager::inputState() {
    return input_state_;
}

}  // namespace vne::events
