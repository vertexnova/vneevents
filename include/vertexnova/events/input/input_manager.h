#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Input manager singleton for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "input_state.h"

#include <utility>

namespace vne::events {

/**
 * @class InputManager
 * @brief Singleton that manages input state and provides static query methods.
 *
 * The InputManager provides a centralized way to query keyboard, mouse,
 * and gamepad states. It wraps an InputState instance and provides
 * convenient static methods for polling.
 *
 * @threadsafe All public methods are thread-safe.
 */
class InputManager {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the InputManager instance.
     *
     * @threadsafe This function is thread-safe.
     */
    static InputManager& instance();

    // Static query methods (convenience wrappers)
    [[nodiscard]] static bool isKeyPressed(int key);
    [[nodiscard]] static bool isKeyJustPressed(int key);
    [[nodiscard]] static bool isKeyJustReleased(int key);

    [[nodiscard]] static bool isMouseButtonPressed(int button);
    [[nodiscard]] static bool isMouseButtonJustPressed(int button);
    [[nodiscard]] static bool isMouseButtonJustReleased(int button);

    [[nodiscard]] static std::pair<int, int> mousePosition();
    [[nodiscard]] static std::pair<float, float> mouseScroll();
    [[nodiscard]] static std::pair<int, int> windowSize();

    /// Resets per-frame input state (call once per frame)
    static void nextFrame();

    /// Gamepad button query (stub for future implementation)
    [[nodiscard]] static bool isGamepadButtonPressed(int gamepad, int button);

    /// Access to the underlying input state (for updates)
    [[nodiscard]] const InputState& inputState() const;
    [[nodiscard]] InputState& inputState();

   private:
    InputManager();
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    InputState input_state_;
};

}  // namespace vne::events
