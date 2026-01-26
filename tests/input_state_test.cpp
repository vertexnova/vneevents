/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include "vertexnova/events/events.h"

namespace vne::events {

// ============================================================================
// InputState Tests
// ============================================================================

TEST(InputStateTest, DefaultState) {
    InputState state;

    EXPECT_FALSE(state.isKeyPressed(static_cast<int>(KeyCode::eA)));
    EXPECT_FALSE(state.isMouseButtonPressed(0));

    auto [mx, my] = state.mousePosition();
    EXPECT_EQ(mx, 0);
    EXPECT_EQ(my, 0);

    auto [sx, sy] = state.mouseScroll();
    EXPECT_FLOAT_EQ(sx, 0.0f);
    EXPECT_FLOAT_EQ(sy, 0.0f);
}

TEST(InputStateTest, KeyPressed) {
    InputState state;
    int key = static_cast<int>(KeyCode::eA);

    state.updateKeyState(key, true);

    EXPECT_TRUE(state.isKeyPressed(key));
    EXPECT_TRUE(state.isKeyJustPressed(key));
    EXPECT_FALSE(state.isKeyJustReleased(key));
}

TEST(InputStateTest, KeyReleased) {
    InputState state;
    int key = static_cast<int>(KeyCode::eA);

    state.updateKeyState(key, true);
    state.nextFrame();
    state.updateKeyState(key, false);

    EXPECT_FALSE(state.isKeyPressed(key));
    EXPECT_FALSE(state.isKeyJustPressed(key));
    EXPECT_TRUE(state.isKeyJustReleased(key));
}

TEST(InputStateTest, JustPressedClearedOnNextFrame) {
    InputState state;
    int key = static_cast<int>(KeyCode::eA);

    state.updateKeyState(key, true);
    EXPECT_TRUE(state.isKeyJustPressed(key));

    state.nextFrame();
    EXPECT_FALSE(state.isKeyJustPressed(key));
    EXPECT_TRUE(state.isKeyPressed(key));  // Still held
}

TEST(InputStateTest, MouseButtonPressed) {
    InputState state;

    state.updateMouseButtonState(0, true);

    EXPECT_TRUE(state.isMouseButtonPressed(0));
    EXPECT_TRUE(state.isMouseButtonJustPressed(0));
}

TEST(InputStateTest, MousePosition) {
    InputState state;

    state.updateMousePosition(100, 200);

    auto [x, y] = state.mousePosition();
    EXPECT_EQ(x, 100);
    EXPECT_EQ(y, 200);
}

TEST(InputStateTest, MouseScroll) {
    InputState state;

    state.updateMouseScroll(0.5f, 1.5f);

    auto [x, y] = state.mouseScroll();
    EXPECT_FLOAT_EQ(x, 0.5f);
    EXPECT_FLOAT_EQ(y, 1.5f);
}

TEST(InputStateTest, MouseScrollClearedOnNextFrame) {
    InputState state;

    state.updateMouseScroll(0.5f, 1.5f);
    state.nextFrame();

    auto [x, y] = state.mouseScroll();
    EXPECT_FLOAT_EQ(x, 0.0f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(InputStateTest, WindowSize) {
    InputState state;

    state.updateWindowSize(1920, 1080);

    auto [w, h] = state.windowSize();
    EXPECT_EQ(w, 1920);
    EXPECT_EQ(h, 1080);
}

// ============================================================================
// InputManager Tests
// ============================================================================

TEST(InputManagerTest, Singleton) {
    auto& manager1 = InputManager::instance();
    auto& manager2 = InputManager::instance();

    EXPECT_EQ(&manager1, &manager2);
}

TEST(InputManagerTest, StaticMethods) {
    auto& manager = InputManager::instance();
    auto& state = manager.inputState();

    int key = static_cast<int>(KeyCode::eSpace);
    state.updateKeyState(key, true);

    EXPECT_TRUE(InputManager::isKeyPressed(key));
    EXPECT_TRUE(InputManager::isKeyJustPressed(key));

    InputManager::nextFrame();
    EXPECT_FALSE(InputManager::isKeyJustPressed(key));

    state.updateKeyState(key, false);
}

TEST(InputManagerTest, GamepadStub) {
    EXPECT_FALSE(InputManager::isGamepadButtonPressed(0, 0));
}

// ============================================================================
// Input Static Interface Tests
// ============================================================================

TEST(InputTest, KeyboardQueries) {
    int key = static_cast<int>(KeyCode::eEnter);

    Input::updateKeyState(key, true);
    EXPECT_TRUE(Input::isKeyPressed(key));

    Input::nextFrame();
    Input::updateKeyState(key, false);
    EXPECT_FALSE(Input::isKeyPressed(key));
}

TEST(InputTest, MouseQueries) {
    Input::updateMousePosition(500, 600);

    auto [x, y] = Input::mousePosition();
    EXPECT_EQ(x, 500);
    EXPECT_EQ(y, 600);
}

TEST(InputTest, WindowQueries) {
    Input::updateWindowSize(2560, 1440);

    auto [w, h] = Input::windowSize();
    EXPECT_EQ(w, 2560);
    EXPECT_EQ(h, 1440);
}

}  // namespace vne::events
