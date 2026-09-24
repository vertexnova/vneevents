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

static_assert(std::atomic<bool>::is_always_lock_free);
static_assert(std::atomic<std::uint64_t>::is_always_lock_free,
              "InputState needs lock-free 64-bit atomics for AtomicXY pairs");

namespace {

constexpr bool isValidKey(int key) noexcept {
    return key >= 0 && key < InputState::kKeyCodeCount;
}

constexpr bool isValidButton(int button) noexcept {
    return button >= 0 && button < InputState::kMouseButtonCount;
}

template<typename Flags>
void clearFlags(Flags& flags) noexcept {
    for (auto& flag : flags) {
        flag.store(false, std::memory_order_relaxed);
    }
}

}  // namespace

InputState::InputState() = default;

bool InputState::isKeyPressed(int key) const {
    return isValidKey(key) && key_state_[static_cast<std::size_t>(key)].load(std::memory_order_relaxed);
}

bool InputState::isKeyJustPressed(int key) const {
    return isValidKey(key) && key_just_pressed_[static_cast<std::size_t>(key)].load(std::memory_order_relaxed);
}

bool InputState::isKeyJustReleased(int key) const {
    return isValidKey(key) && key_just_released_[static_cast<std::size_t>(key)].load(std::memory_order_relaxed);
}

bool InputState::isMouseButtonPressed(int button) const {
    return isValidButton(button) &&
           mouse_button_state_[static_cast<std::size_t>(button)].load(std::memory_order_relaxed);
}

bool InputState::isMouseButtonJustPressed(int button) const {
    return isValidButton(button) &&
           mouse_button_just_pressed_[static_cast<std::size_t>(button)].load(std::memory_order_relaxed);
}

bool InputState::isMouseButtonJustReleased(int button) const {
    return isValidButton(button) &&
           mouse_button_just_released_[static_cast<std::size_t>(button)].load(std::memory_order_relaxed);
}

std::pair<int, int> InputState::mousePosition() const {
    return mouse_position_.load();
}

std::pair<float, float> InputState::mouseScroll() const {
    return mouse_scroll_.load();
}

std::pair<int, int> InputState::windowSize() const {
    return window_size_.load();
}

void InputState::updateKeyState(int key, bool pressed) {
    if (!isValidKey(key)) {
        return;
    }
    const auto i = static_cast<std::size_t>(key);
    const bool was_pressed = key_state_[i].exchange(pressed, std::memory_order_relaxed);
    if (pressed && !was_pressed) {
        key_just_pressed_[i].store(true, std::memory_order_relaxed);
    } else if (!pressed && was_pressed) {
        key_just_released_[i].store(true, std::memory_order_relaxed);
    }
}

void InputState::updateMouseButtonState(int button, bool pressed) {
    if (!isValidButton(button)) {
        return;
    }
    const auto i = static_cast<std::size_t>(button);
    const bool was_pressed = mouse_button_state_[i].exchange(pressed, std::memory_order_relaxed);
    if (pressed && !was_pressed) {
        mouse_button_just_pressed_[i].store(true, std::memory_order_relaxed);
    } else if (!pressed && was_pressed) {
        mouse_button_just_released_[i].store(true, std::memory_order_relaxed);
    }
}

void InputState::updateMousePosition(int x, int y) {
    mouse_position_.store(x, y);
}

void InputState::updateMouseScroll(float x_offset, float y_offset) {
    mouse_scroll_.store(x_offset, y_offset);
}

void InputState::updateWindowSize(int width, int height) {
    window_size_.store(width, height);
}

void InputState::nextFrame() {
    clearFlags(key_just_pressed_);
    clearFlags(key_just_released_);
    clearFlags(mouse_button_just_pressed_);
    clearFlags(mouse_button_just_released_);
    mouse_scroll_.reset();
}

}  // namespace vne::events
