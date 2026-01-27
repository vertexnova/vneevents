#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener classes for Hello Events example
 * ----------------------------------------------------------------------
 */

#include <vertexnova/events/events.h>

namespace vne::events::examples {

/**
 * @class KeyboardListener
 * @brief Listener for keyboard events (key pressed/released).
 */
class KeyboardListener : public vne::events::EventListener {
   public:
    KeyboardListener() = default;
    ~KeyboardListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class MouseListener
 * @brief Listener for mouse events (movement, button presses, scrolling).
 */
class MouseListener : public vne::events::EventListener {
   public:
    MouseListener() = default;
    ~MouseListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class WindowListener
 * @brief Listener for window events (resize, close).
 */
class WindowListener : public vne::events::EventListener {
   public:
    WindowListener() = default;
    ~WindowListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

}  // namespace vne::events::examples
