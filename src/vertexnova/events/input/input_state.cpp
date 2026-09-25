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
              "InputState needs lock-free 64-bit atomics for AtomicXY / frame ids");

namespace {

constexpr bool isValidKey(int key) noexcept {
    return key >= 0 && key < InputState::kKeyCodeCount;
}

constexpr bool isValidButton(int button) noexcept {
    return button >= 0 && button < InputState::kMouseButtonCount;
}

/// Write the current frame id into @p slot; retry if nextFrame() advanced mid-write.
void setFrameId(std::atomic<std::uint64_t>& slot, const std::atomic<std::uint64_t>& frame) noexcept {
    for (;;) {
        const auto current = frame.load(std::memory_order_relaxed);
        slot.store(current, std::memory_order_release);
        if (frame.load(std::memory_order_acquire) == current) {
            return;
        }
    }
}

[[nodiscard]] bool isCurrentFrame(const std::atomic<std::uint64_t>& slot,
                                  const std::atomic<std::uint64_t>& frame) noexcept {
    const auto current = frame.load(std::memory_order_acquire);
    return slot.load(std::memory_order_acquire) == current;
}

}  // namespace

InputState::InputState() = default;

bool InputState::isKeyPressed(int key) const {
    return isValidKey(key) && key_state_[static_cast<std::size_t>(key)].load(std::memory_order_relaxed);
}

bool InputState::isKeyJustPressed(int key) const {
    return isValidKey(key) && isCurrentFrame(key_just_pressed_[static_cast<std::size_t>(key)], frame_);
}

bool InputState::isKeyJustReleased(int key) const {
    return isValidKey(key) && isCurrentFrame(key_just_released_[static_cast<std::size_t>(key)], frame_);
}

bool InputState::isMouseButtonPressed(int button) const {
    return isValidButton(button)
           && mouse_button_state_[static_cast<std::size_t>(button)].load(std::memory_order_relaxed);
}

bool InputState::isMouseButtonJustPressed(int button) const {
    return isValidButton(button)
           && isCurrentFrame(mouse_button_just_pressed_[static_cast<std::size_t>(button)], frame_);
}

bool InputState::isMouseButtonJustReleased(int button) const {
    return isValidButton(button)
           && isCurrentFrame(mouse_button_just_released_[static_cast<std::size_t>(button)], frame_);
}

std::pair<int, int> InputState::mousePosition() const {
    return mouse_position_.load();
}

std::pair<float, float> InputState::mouseScroll() const {
    const auto current = frame_.load(std::memory_order_acquire);
    if (mouse_scroll_frame_.load(std::memory_order_acquire) != current) {
        return {0.0f, 0.0f};
    }
    const auto value = mouse_scroll_.load();
    // Drop the sample if nextFrame() raced between the frame-id check and the load.
    if (mouse_scroll_frame_.load(std::memory_order_acquire) != current) {
        return {0.0f, 0.0f};
    }
    return value;
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
        setFrameId(key_just_pressed_[i], frame_);
    } else if (!pressed && was_pressed) {
        setFrameId(key_just_released_[i], frame_);
    }
}

void InputState::updateMouseButtonState(int button, bool pressed) {
    if (!isValidButton(button)) {
        return;
    }
    const auto i = static_cast<std::size_t>(button);
    const bool was_pressed = mouse_button_state_[i].exchange(pressed, std::memory_order_relaxed);
    if (pressed && !was_pressed) {
        setFrameId(mouse_button_just_pressed_[i], frame_);
    } else if (!pressed && was_pressed) {
        setFrameId(mouse_button_just_released_[i], frame_);
    }
}

void InputState::updateMousePosition(int x, int y) {
    mouse_position_.store(x, y);
}

void InputState::updateMouseScroll(float x_offset, float y_offset) {
    for (;;) {
        const auto current = frame_.load(std::memory_order_relaxed);
        mouse_scroll_.store(x_offset, y_offset);
        mouse_scroll_frame_.store(current, std::memory_order_release);
        if (frame_.load(std::memory_order_acquire) == current) {
            return;
        }
    }
}

void InputState::updateWindowSize(int width, int height) {
    window_size_.store(width, height);
}

void InputState::nextFrame() {
    // Advance epoch: prior edge/scroll frame ids no longer match. Concurrent writers
    // that observe the bump rewrite onto this new frame instead of being erased.
    frame_.fetch_add(1, std::memory_order_acq_rel);
}

}  // namespace vne::events
