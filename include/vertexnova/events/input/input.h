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

#include "../export.h"

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
class VNEEVENTS_API Input {
   public:
    // Keyboard
    [[nodiscard]] static bool isKeyPressed(int key);
    [[nodiscard]] static bool isKeyJustPressed(int key);
    [[nodiscard]] static bool isKeyJustReleased(int key);

    // Mouse buttons
    [[nodiscard]] static bool isMouseButtonPressed(int button);
    [[nodiscard]] static bool isMouseButtonJustPressed(int button);
    [[nodiscard]] static bool isMouseButtonJustReleased(int button);

    // Mouse position and scroll
    [[nodiscard]] static std::pair<int, int> mousePosition();
    [[nodiscard]] static std::pair<float, float> mouseScroll();

    // Window size
    [[nodiscard]] static std::pair<int, int> windowSize();

    // Per-frame reset
    static void nextFrame();

    // Gamepad (stub)
    [[nodiscard]] static bool isGamepadButtonPressed(int gamepad, int button);

    // Update methods for platform integration
    static void updateKeyState(int key, bool pressed);
    static void updateMouseButtonState(int button, bool pressed);
    static void updateMousePosition(int x, int y);
    static void updateMouseScroll(float x_offset, float y_offset);
    static void updateWindowSize(int width, int height);
};

}  // namespace vne::events
