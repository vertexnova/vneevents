/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include "vertexnova/events/events.h"

namespace vne::events {

// ============================================================================
// Event Base Class Tests
// ============================================================================

TEST(EventTest, KeyPressedEventCreation) {
    KeyPressedEvent event(KeyCode::eA);

    EXPECT_EQ(event.type(), EventType::eKeyPressed);
    EXPECT_EQ(event.keyCode(), KeyCode::eA);
    EXPECT_EQ(event.name(), "KeyPressed");
    EXPECT_FALSE(event.isHandled());
}

TEST(EventTest, KeyPressedEventWithModifiers) {
    KeyPressedEvent event(KeyCode::eC, ModifierKey::eModCtrl);

    EXPECT_EQ(event.keyCode(), KeyCode::eC);
    EXPECT_EQ(event.modifiers(), ModifierKey::eModCtrl);
}

TEST(EventTest, KeyReleasedEvent) {
    KeyReleasedEvent event(KeyCode::eSpace);

    EXPECT_EQ(event.type(), EventType::eKeyReleased);
    EXPECT_EQ(event.keyCode(), KeyCode::eSpace);
    EXPECT_EQ(event.name(), "KeyReleased");
}

TEST(EventTest, KeyRepeatEvent) {
    KeyRepeatEvent event(KeyCode::eA, 5);

    EXPECT_EQ(event.type(), EventType::eKeyRepeat);
    EXPECT_EQ(event.repeatCount(), 5u);
}

TEST(EventTest, KeyTypedEvent) {
    KeyTypedEvent event(KeyCode::eA);

    EXPECT_EQ(event.type(), EventType::eKeyTyped);
    EXPECT_EQ(event.name(), "KeyTyped");
}

TEST(EventTest, EventCategoryFlags) {
    KeyPressedEvent key_event(KeyCode::eA);
    EXPECT_TRUE(key_event.isInCategory(EventCategory::eKeyboard));
    EXPECT_TRUE(key_event.isInCategory(EventCategory::eInput));
    EXPECT_FALSE(key_event.isInCategory(EventCategory::eMouse));
}

TEST(EventTest, EventHandled) {
    KeyPressedEvent event(KeyCode::eA);

    EXPECT_FALSE(event.isHandled());
    event.setHandled();
    EXPECT_TRUE(event.isHandled());
}

TEST(EventTest, EventTimestamp) {
    auto before = std::chrono::system_clock::now();
    KeyPressedEvent event(KeyCode::eA);
    auto after = std::chrono::system_clock::now();

    EXPECT_GE(event.timestamp(), before);
    EXPECT_LE(event.timestamp(), after);
}

// ============================================================================
// Mouse Event Tests
// ============================================================================

TEST(MouseEventTest, MouseButtonPressed) {
    MouseButtonPressedEvent event(MouseButton::eLeft);

    EXPECT_EQ(event.type(), EventType::eMouseButtonPressed);
    EXPECT_EQ(event.button(), MouseButton::eLeft);
    EXPECT_FALSE(event.hasPosition());
    EXPECT_TRUE(std::isnan(event.x()));
    EXPECT_TRUE(std::isnan(event.y()));
    EXPECT_TRUE(event.isInCategory(EventCategory::eMouseButton));
}

TEST(MouseEventTest, MouseButtonPressedWithPosition) {
    MouseButtonPressedEvent event(MouseButton::eLeft, uint8_t{0}, 42.5, 100.25);

    EXPECT_EQ(event.button(), MouseButton::eLeft);
    EXPECT_TRUE(event.hasPosition());
    EXPECT_DOUBLE_EQ(event.x(), 42.5);
    EXPECT_DOUBLE_EQ(event.y(), 100.25);
    EXPECT_NE(event.toString().find(" at (42.5, 100.25)"), std::string::npos);
}

TEST(MouseEventTest, MouseButtonPressedToStringOmitsPositionWhenUnknown) {
    MouseButtonPressedEvent event(MouseButton::eLeft);
    EXPECT_FALSE(event.hasPosition());
    EXPECT_EQ(event.toString().find(" at ("), std::string::npos);
}

TEST(MouseEventTest, MouseButtonReleased) {
    MouseButtonReleasedEvent event(MouseButton::eRight);

    EXPECT_EQ(event.type(), EventType::eMouseButtonReleased);
    EXPECT_EQ(event.button(), MouseButton::eRight);
    EXPECT_FALSE(event.hasPosition());
    EXPECT_TRUE(std::isnan(event.x()));
    EXPECT_TRUE(std::isnan(event.y()));
}

TEST(MouseEventTest, MouseButtonReleasedWithPosition) {
    MouseButtonReleasedEvent event(MouseButton::eRight, uint8_t{0}, 10.0, 20.0);

    EXPECT_EQ(event.button(), MouseButton::eRight);
    EXPECT_TRUE(event.hasPosition());
    EXPECT_DOUBLE_EQ(event.x(), 10.0);
    EXPECT_DOUBLE_EQ(event.y(), 20.0);
    EXPECT_NE(event.toString().find(" at (10, 20)"), std::string::npos);
}

TEST(MouseEventTest, MouseButtonReleasedToStringOmitsPositionWhenUnknown) {
    MouseButtonReleasedEvent event(MouseButton::eRight);
    EXPECT_FALSE(event.hasPosition());
    EXPECT_EQ(event.toString().find(" at ("), std::string::npos);
}

TEST(MouseEventTest, MouseButtonDoubleClicked) {
    MouseButtonDoubleClickedEvent event(MouseButton::eLeft);

    EXPECT_EQ(event.type(), EventType::eMouseButtonDoubleClicked);
    EXPECT_EQ(event.button(), MouseButton::eLeft);
    EXPECT_FALSE(event.hasPosition());
    EXPECT_TRUE(std::isnan(event.x()));
    EXPECT_TRUE(std::isnan(event.y()));
}

TEST(MouseEventTest, MouseButtonDoubleClickedWithPosition) {
    MouseButtonDoubleClickedEvent event(MouseButton::eLeft, uint8_t{0}, 12.5, 34.75);

    EXPECT_EQ(event.button(), MouseButton::eLeft);
    EXPECT_TRUE(event.hasPosition());
    EXPECT_DOUBLE_EQ(event.x(), 12.5);
    EXPECT_DOUBLE_EQ(event.y(), 34.75);
    EXPECT_NE(event.toString().find(" at (12.5, 34.75)"), std::string::npos);
}

TEST(MouseEventTest, MouseButtonDoubleClickedToStringOmitsPositionWhenUnknown) {
    MouseButtonDoubleClickedEvent event(MouseButton::eLeft);
    EXPECT_FALSE(event.hasPosition());
    EXPECT_EQ(event.toString().find(" at ("), std::string::npos);
}

TEST(MouseEventTest, MouseMoved) {
    MouseMovedEvent event(100.5, 200.5);

    EXPECT_EQ(event.type(), EventType::eMouseMoved);
    EXPECT_DOUBLE_EQ(event.x(), 100.5);
    EXPECT_DOUBLE_EQ(event.y(), 200.5);
    EXPECT_TRUE(event.isInCategory(EventCategory::eMouse));
}

TEST(MouseEventTest, MouseScrolled) {
    MouseScrolledEvent event(0.0, 1.5);

    EXPECT_EQ(event.type(), EventType::eMouseScrolled);
    EXPECT_DOUBLE_EQ(event.xOffset(), 0.0);
    EXPECT_DOUBLE_EQ(event.yOffset(), 1.5);
}

// ============================================================================
// Window Event Tests
// ============================================================================

TEST(WindowEventTest, WindowClose) {
    WindowCloseEvent event;

    EXPECT_EQ(event.type(), EventType::eWindowClose);
    EXPECT_TRUE(event.isInCategory(EventCategory::eWindow));
    EXPECT_FALSE(event.isInCategory(EventCategory::eInput));
}

TEST(WindowEventTest, WindowResize) {
    WindowResizeEvent event(1920, 1080);

    EXPECT_EQ(event.type(), EventType::eWindowResize);
    EXPECT_EQ(event.width(), 1920u);
    EXPECT_EQ(event.height(), 1080u);
}

TEST(WindowEventTest, WindowFocusGained) {
    WindowFocusEvent event(true);

    EXPECT_EQ(event.type(), EventType::eWindowFocus);
    EXPECT_TRUE(event.focused());
    EXPECT_TRUE(event.isInCategory(EventCategory::eWindow));
    EXPECT_FALSE(event.isInCategory(EventCategory::eInput));
    EXPECT_EQ(event.name(), "WindowFocus");
    EXPECT_EQ(event.toString(), "WindowFocusEvent: gained");
}

TEST(WindowEventTest, WindowFocusLost) {
    WindowFocusEvent event(false);

    EXPECT_EQ(event.type(), EventType::eWindowFocus);
    EXPECT_FALSE(event.focused());
    EXPECT_TRUE(event.isInCategory(EventCategory::eWindow));
    EXPECT_FALSE(event.isInCategory(EventCategory::eInput));
    EXPECT_EQ(event.name(), "WindowFocus");
    EXPECT_EQ(event.toString(), "WindowFocusEvent: lost");
}

// ============================================================================
// Touch Event Tests
// ============================================================================

TEST(TouchEventTest, TouchPress) {
    TouchPressEvent event(0, 100.0, 200.0);

    EXPECT_EQ(event.type(), EventType::eTouchPress);
    EXPECT_EQ(event.touchId(), 0u);
    EXPECT_DOUBLE_EQ(event.x(), 100.0);
    EXPECT_DOUBLE_EQ(event.y(), 200.0);
    EXPECT_TRUE(event.isInCategory(EventCategory::eTouchScreen));
}

TEST(TouchEventTest, TouchRelease) {
    TouchReleaseEvent event(1, 150.0, 250.0);

    EXPECT_EQ(event.type(), EventType::eTouchRelease);
    EXPECT_EQ(event.touchId(), 1u);
}

TEST(TouchEventTest, TouchMove) {
    TouchMoveEvent event(0, 110.0, 210.0);

    EXPECT_EQ(event.type(), EventType::eTouchMove);
}

// ============================================================================
// Event ToString Tests
// ============================================================================

TEST(EventToStringTest, KeyPressedToString) {
    KeyPressedEvent event(KeyCode::eA);
    std::string str = event.toString();

    EXPECT_FALSE(str.empty());
    EXPECT_NE(str.find("KeyPressed"), std::string::npos);
}

TEST(EventToStringTest, MouseMovedToString) {
    MouseMovedEvent event(100.0, 200.0);
    std::string str = event.toString();

    EXPECT_NE(str.find("100"), std::string::npos);
    EXPECT_NE(str.find("200"), std::string::npos);
}

}  // namespace vne::events
