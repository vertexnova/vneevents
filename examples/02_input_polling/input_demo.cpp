/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Input polling demonstration implementation
 * ----------------------------------------------------------------------
 */

#include "input_demo.h"

#include "common/logging_guard.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

namespace vne::events::examples {

void InputDemo::run() {
    VNE_LOG_INFO << "=== Input Polling Demonstration ===";
    VNE_LOG_INFO << "";

    demonstrateKeyboardPolling();
    demonstrateMousePolling();
    demonstratePerFrameState();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demonstration Complete ===";
}

void InputDemo::demonstrateKeyboardPolling() {
    VNE_LOG_INFO << "--- Keyboard Polling ---";

    // Simulate key presses by updating input state
    VNE_LOG_INFO << "  Simulating key presses...";

    // Press W key
    vne::events::Input::updateKeyState(static_cast<int>(vne::events::KeyCode::eW), true);
    VNE_LOG_INFO << "  W key pressed";
    VNE_LOG_INFO << "    isKeyPressed(W): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eW)) ? "true" : "false");
    VNE_LOG_INFO << "    isKeyJustPressed(W): " << (vne::events::Input::isKeyJustPressed(static_cast<int>(vne::events::KeyCode::eW)) ? "true" : "false");

    // Press A key
    vne::events::Input::updateKeyState(static_cast<int>(vne::events::KeyCode::eA), true);
    VNE_LOG_INFO << "  A key pressed";
    VNE_LOG_INFO << "    isKeyPressed(A): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eA)) ? "true" : "false");
    VNE_LOG_INFO << "    isKeyJustPressed(A): " << (vne::events::Input::isKeyJustPressed(static_cast<int>(vne::events::KeyCode::eA)) ? "true" : "false");

    // Release W key
    vne::events::Input::updateKeyState(static_cast<int>(vne::events::KeyCode::eW), false);
    VNE_LOG_INFO << "  W key released";
    VNE_LOG_INFO << "    isKeyPressed(W): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eW)) ? "true" : "false");
    VNE_LOG_INFO << "    isKeyJustReleased(W): " << (vne::events::Input::isKeyJustReleased(static_cast<int>(vne::events::KeyCode::eW)) ? "true" : "false");

    VNE_LOG_INFO << "";
}

void InputDemo::demonstrateMousePolling() {
    VNE_LOG_INFO << "--- Mouse Polling ---";

    // Simulate mouse button press
    VNE_LOG_INFO << "  Simulating mouse input...";

    // Update mouse position
    vne::events::Input::updateMousePosition(100, 200);
    auto [x, y] = vne::events::Input::mousePosition();
    VNE_LOG_INFO << "  Mouse position: (" << x << ", " << y << ")";

    // Press left mouse button
    vne::events::Input::updateMouseButtonState(static_cast<int>(vne::events::MouseButton::eLeft), true);
    VNE_LOG_INFO << "  Left mouse button pressed";
    VNE_LOG_INFO << "    isMouseButtonPressed(Left): " << (vne::events::Input::isMouseButtonPressed(static_cast<int>(vne::events::MouseButton::eLeft)) ? "true" : "false");
    VNE_LOG_INFO << "    isMouseButtonJustPressed(Left): " << (vne::events::Input::isMouseButtonJustPressed(static_cast<int>(vne::events::MouseButton::eLeft)) ? "true" : "false");

    // Update mouse position again
    vne::events::Input::updateMousePosition(150, 250);
    auto [x2, y2] = vne::events::Input::mousePosition();
    VNE_LOG_INFO << "  Mouse moved to: (" << x2 << ", " << y2 << ")";

    // Simulate mouse scroll
    vne::events::Input::updateMouseScroll(0.0f, 1.0f);
    auto [scroll_x, scroll_y] = vne::events::Input::mouseScroll();
    VNE_LOG_INFO << "  Mouse scrolled: (" << scroll_x << ", " << scroll_y << ")";

    // Release left mouse button
    vne::events::Input::updateMouseButtonState(static_cast<int>(vne::events::MouseButton::eLeft), false);
    VNE_LOG_INFO << "  Left mouse button released";
    VNE_LOG_INFO << "    isMouseButtonPressed(Left): " << (vne::events::Input::isMouseButtonPressed(static_cast<int>(vne::events::MouseButton::eLeft)) ? "true" : "false");
    VNE_LOG_INFO << "    isMouseButtonJustReleased(Left): " << (vne::events::Input::isMouseButtonJustReleased(static_cast<int>(vne::events::MouseButton::eLeft)) ? "true" : "false");

    VNE_LOG_INFO << "";
}

void InputDemo::demonstratePerFrameState() {
    VNE_LOG_INFO << "--- Per-Frame State Management ---";

    VNE_LOG_INFO << "  Frame 1:";
    // Simulate frame 1
    vne::events::Input::updateKeyState(static_cast<int>(vne::events::KeyCode::eSpace), true);
    VNE_LOG_INFO << "    Space key pressed";
    VNE_LOG_INFO << "      isKeyJustPressed(Space): " << (vne::events::Input::isKeyJustPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");
    VNE_LOG_INFO << "      isKeyPressed(Space): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");

    // End of frame - reset just-pressed states
    vne::events::Input::nextFrame();
    VNE_LOG_INFO << "    After nextFrame():";
    VNE_LOG_INFO << "      isKeyJustPressed(Space): " << (vne::events::Input::isKeyJustPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");
    VNE_LOG_INFO << "      isKeyPressed(Space): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "  Frame 2:";
    // Frame 2 - key still held
    VNE_LOG_INFO << "    Space key still held";
    VNE_LOG_INFO << "      isKeyJustPressed(Space): " << (vne::events::Input::isKeyJustPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");
    VNE_LOG_INFO << "      isKeyPressed(Space): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");

    // Release key
    vne::events::Input::updateKeyState(static_cast<int>(vne::events::KeyCode::eSpace), false);
    VNE_LOG_INFO << "    Space key released";
    VNE_LOG_INFO << "      isKeyJustReleased(Space): " << (vne::events::Input::isKeyJustReleased(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");

    // End of frame
    vne::events::Input::nextFrame();
    VNE_LOG_INFO << "    After nextFrame():";
    VNE_LOG_INFO << "      isKeyJustReleased(Space): " << (vne::events::Input::isKeyJustReleased(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");
    VNE_LOG_INFO << "      isKeyPressed(Space): " << (vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eSpace)) ? "true" : "false");

    VNE_LOG_INFO << "";
}

}  // namespace vne::events::examples
