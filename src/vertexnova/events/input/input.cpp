/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 * ----------------------------------------------------------------------
 */

#include "vertexnova/events/input/input.h"

#include "vertexnova/events/input/input_manager.h"

namespace vne::events {

bool Input::isKeyPressed(int key) {
    return InputManager::isKeyPressed(key);
}

bool Input::isKeyJustPressed(int key) {
    return InputManager::isKeyJustPressed(key);
}

bool Input::isKeyJustReleased(int key) {
    return InputManager::isKeyJustReleased(key);
}

bool Input::isMouseButtonPressed(int button) {
    return InputManager::isMouseButtonPressed(button);
}

bool Input::isMouseButtonJustPressed(int button) {
    return InputManager::isMouseButtonJustPressed(button);
}

bool Input::isMouseButtonJustReleased(int button) {
    return InputManager::isMouseButtonJustReleased(button);
}

std::pair<int, int> Input::mousePosition() {
    return InputManager::mousePosition();
}

std::pair<float, float> Input::mouseScroll() {
    return InputManager::mouseScroll();
}

std::pair<int, int> Input::windowSize() {
    return InputManager::windowSize();
}

void Input::nextFrame() {
    InputManager::nextFrame();
}

bool Input::isGamepadButtonPressed(int gamepad, int button) {
    return InputManager::isGamepadButtonPressed(gamepad, button);
}

void Input::updateKeyState(int key, bool pressed) {
    InputManager::instance().inputState().updateKeyState(key, pressed);
}

void Input::updateMouseButtonState(int button, bool pressed) {
    InputManager::instance().inputState().updateMouseButtonState(button, pressed);
}

void Input::updateMousePosition(int x, int y) {
    InputManager::instance().inputState().updateMousePosition(x, y);
}

void Input::updateMouseScroll(float x_offset, float y_offset) {
    InputManager::instance().inputState().updateMouseScroll(x_offset, y_offset);
}

void Input::updateWindowSize(int width, int height) {
    InputManager::instance().inputState().updateWindowSize(width, height);
}

}  // namespace vne::events
