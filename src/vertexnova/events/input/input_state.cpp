/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 * ----------------------------------------------------------------------
 */

#include "vertexnova/events/input/input_state.h"

namespace vne::events {

InputState::InputState() = default;

bool InputState::isKeyPressed(int key) const {
    internal::ReadLockGuard lock(mutex_);
    auto it = key_state_.find(key);
    return it != key_state_.end() && it->second;
}

bool InputState::isKeyJustPressed(int key) const {
    internal::ReadLockGuard lock(mutex_);
    auto it = key_just_pressed_.find(key);
    return it != key_just_pressed_.end() && it->second;
}

bool InputState::isKeyJustReleased(int key) const {
    internal::ReadLockGuard lock(mutex_);
    auto it = key_just_released_.find(key);
    return it != key_just_released_.end() && it->second;
}

bool InputState::isMouseButtonPressed(int button) const {
    internal::ReadLockGuard lock(mutex_);
    auto it = mouse_button_state_.find(button);
    return it != mouse_button_state_.end() && it->second;
}

bool InputState::isMouseButtonJustPressed(int button) const {
    internal::ReadLockGuard lock(mutex_);
    auto it = mouse_button_just_pressed_.find(button);
    return it != mouse_button_just_pressed_.end() && it->second;
}

bool InputState::isMouseButtonJustReleased(int button) const {
    internal::ReadLockGuard lock(mutex_);
    auto it = mouse_button_just_released_.find(button);
    return it != mouse_button_just_released_.end() && it->second;
}

std::pair<int, int> InputState::mousePosition() const {
    internal::ReadLockGuard lock(mutex_);
    return mouse_position_;
}

std::pair<float, float> InputState::mouseScroll() const {
    internal::ReadLockGuard lock(mutex_);
    return mouse_scroll_;
}

std::pair<int, int> InputState::windowSize() const {
    internal::ReadLockGuard lock(mutex_);
    return window_size_;
}

void InputState::updateKeyState(int key, bool pressed) {
    internal::WriteLockGuard lock(mutex_);
    bool was_pressed = key_state_[key];
    key_state_[key] = pressed;

    if (pressed && !was_pressed) {
        key_just_pressed_[key] = true;
    } else if (!pressed && was_pressed) {
        key_just_released_[key] = true;
    }
}

void InputState::updateMouseButtonState(int button, bool pressed) {
    internal::WriteLockGuard lock(mutex_);
    bool was_pressed = mouse_button_state_[button];
    mouse_button_state_[button] = pressed;

    if (pressed && !was_pressed) {
        mouse_button_just_pressed_[button] = true;
    } else if (!pressed && was_pressed) {
        mouse_button_just_released_[button] = true;
    }
}

void InputState::updateMousePosition(int x, int y) {
    internal::WriteLockGuard lock(mutex_);
    mouse_position_ = {x, y};
}

void InputState::updateMouseScroll(float x_offset, float y_offset) {
    internal::WriteLockGuard lock(mutex_);
    mouse_scroll_ = {x_offset, y_offset};
}

void InputState::updateWindowSize(int width, int height) {
    internal::WriteLockGuard lock(mutex_);
    window_size_ = {width, height};
}

void InputState::nextFrame() {
    internal::WriteLockGuard lock(mutex_);
    key_just_pressed_.clear();
    key_just_released_.clear();
    mouse_button_just_pressed_.clear();
    mouse_button_just_released_.clear();
    mouse_scroll_ = {0.0f, 0.0f};
}

}  // namespace vne::events
