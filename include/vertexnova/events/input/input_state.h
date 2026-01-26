#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Input state tracking for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "../internal/read_write_mutex.h"

#include <unordered_map>
#include <utility>

namespace vne::events {

class InputManager;

/**
 * @class InputState
 * @brief Manages the state of keyboard, mouse, and window inputs.
 *
 * This class tracks key states, mouse button states, mouse position,
 * scroll offsets, and window size. It provides thread-safe access
 * to query and update these states.
 *
 * @threadsafe All public methods are thread-safe.
 */
class InputState {
   public:
    InputState();

    // Key state queries
    [[nodiscard]] bool isKeyPressed(int key) const;
    [[nodiscard]] bool isKeyJustPressed(int key) const;
    [[nodiscard]] bool isKeyJustReleased(int key) const;

    // Mouse button queries
    [[nodiscard]] bool isMouseButtonPressed(int button) const;
    [[nodiscard]] bool isMouseButtonJustPressed(int button) const;
    [[nodiscard]] bool isMouseButtonJustReleased(int button) const;

    // Mouse position and scroll
    [[nodiscard]] std::pair<int, int> mousePosition() const;
    [[nodiscard]] std::pair<float, float> mouseScroll() const;

    // Window size
    [[nodiscard]] std::pair<int, int> windowSize() const;

    // State updates (called by input system)
    void updateKeyState(int key, bool pressed);
    void updateMouseButtonState(int button, bool pressed);
    void updateMousePosition(int x, int y);
    void updateMouseScroll(float x_offset, float y_offset);
    void updateWindowSize(int width, int height);

    /// Called once per frame to reset just-pressed/released states
    void nextFrame();

   private:
    mutable internal::ReadWriteMutex mutex_;

    std::unordered_map<int, bool> key_state_;
    std::unordered_map<int, bool> key_just_pressed_;
    std::unordered_map<int, bool> key_just_released_;

    std::unordered_map<int, bool> mouse_button_state_;
    std::unordered_map<int, bool> mouse_button_just_pressed_;
    std::unordered_map<int, bool> mouse_button_just_released_;

    std::pair<int, int> mouse_position_{0, 0};
    std::pair<float, float> mouse_scroll_{0.0f, 0.0f};
    std::pair<int, int> window_size_{0, 0};

    friend class InputManager;
};

}  // namespace vne::events
