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

// ============================================================================
// Window Identity Tests
// ============================================================================

TEST(WindowIdTest, DefaultsToInvalid) {
    KeyPressedEvent key(KeyCode::eA);
    WindowResizeEvent resize(800, 600);
    TouchPressEvent touch(1U, 10.0, 20.0);

    EXPECT_EQ(key.windowId(), kInvalidWindowId);
    EXPECT_EQ(resize.windowId(), kInvalidWindowId);
    EXPECT_EQ(touch.windowId(), kInvalidWindowId);
}

TEST(WindowIdTest, RoundTripsThroughEveryFamily) {
    constexpr WindowId kId = 42U;

    EXPECT_EQ(KeyPressedEvent(KeyCode::eA, 0, kId).windowId(), kId);
    EXPECT_EQ(KeyReleasedEvent(KeyCode::eA, 0, kId).windowId(), kId);
    EXPECT_EQ(KeyRepeatEvent(KeyCode::eA, 3U, 0, kId).windowId(), kId);
    EXPECT_EQ(TextInputEvent("x", kId).windowId(), kId);
    EXPECT_EQ(MouseButtonPressedEvent(MouseButton::eLeft, 0, 1.0, 2.0, kId).windowId(), kId);
    EXPECT_EQ(MouseMovedEvent(1.0, 2.0, 0, kId).windowId(), kId);
    EXPECT_EQ(MouseScrolledEvent(0.0, 1.0, 3.0, 4.0, 0, kId).windowId(), kId);
    EXPECT_EQ(TouchMoveEvent(1U, 1.0, 2.0, 0, kId).windowId(), kId);
    EXPECT_EQ(WindowResizeEvent(800, 600, kId).windowId(), kId);
    EXPECT_EQ(WindowCloseEvent(kId).windowId(), kId);
    EXPECT_EQ(WindowFocusEvent(true, kId).windowId(), kId);
}

TEST(WindowIdTest, ToStringOmitsSuffixWhenUnset) {
    // Debug output must stay byte-identical to pre-WindowId releases for unstamped events.
    EXPECT_EQ(WindowCloseEvent().toString(), "WindowCloseEvent");
    EXPECT_EQ(WindowFocusEvent(true).toString(), "WindowFocusEvent: gained");
    EXPECT_EQ(WindowResizeEvent(1920, 1080).toString(), "WindowResizeEvent: (1920, 1080)");
}

TEST(WindowIdTest, ToStringAppendsSuffixWhenSet) {
    EXPECT_EQ(WindowCloseEvent(7U).toString(), "WindowCloseEvent [window 7]");
    EXPECT_EQ(WindowFocusEvent(false, 7U).toString(), "WindowFocusEvent: lost [window 7]");
    EXPECT_EQ(WindowResizeEvent(1920, 1080, 7U).toString(), "WindowResizeEvent: (1920, 1080) [window 7]");
}

// ============================================================================
// Window State Event Tests
// ============================================================================

TEST(WindowStateEventTest, Minimize) {
    WindowMinimizeEvent event(3U);

    EXPECT_EQ(event.type(), EventType::eWindowMinimize);
    EXPECT_EQ(event.name(), "WindowMinimize");
    EXPECT_EQ(event.windowId(), 3U);
    EXPECT_TRUE(event.isInCategory(EventCategory::eWindow));
    EXPECT_FALSE(event.isInCategory(EventCategory::eInput));
}

TEST(WindowStateEventTest, Restore) {
    WindowRestoreEvent event(3U);

    EXPECT_EQ(event.type(), EventType::eWindowRestore);
    EXPECT_EQ(event.name(), "WindowRestore");
    EXPECT_TRUE(event.isInCategory(EventCategory::eWindow));
}

TEST(WindowStateEventTest, Move) {
    WindowMoveEvent event(-40, 120);

    EXPECT_EQ(event.type(), EventType::eWindowMove);
    EXPECT_EQ(event.x(), -40);
    EXPECT_EQ(event.y(), 120);
    EXPECT_EQ(event.toString(), "WindowMoveEvent: (-40, 120)");
}

TEST(WindowStateEventTest, DpiChanged) {
    WindowDpiChangedEvent event(2.0F);

    EXPECT_EQ(event.type(), EventType::eWindowDpiChanged);
    EXPECT_FLOAT_EQ(event.scale(), 2.0F);
    EXPECT_TRUE(event.isInCategory(EventCategory::eWindow));
}

TEST(WindowStateEventTest, SafeAreaChanged) {
    WindowSafeAreaChangedEvent event(59.0F, 0.0F, 34.0F, 0.0F);

    EXPECT_EQ(event.type(), EventType::eWindowSafeAreaChanged);
    EXPECT_FLOAT_EQ(event.top(), 59.0F);
    EXPECT_FLOAT_EQ(event.left(), 0.0F);
    EXPECT_FLOAT_EQ(event.bottom(), 34.0F);
    EXPECT_FLOAT_EQ(event.right(), 0.0F);
}

// ============================================================================
// Application Lifecycle Event Tests
// ============================================================================

TEST(ApplicationEventTest, PauseResumeLowMemory) {
    ApplicationPauseEvent pause;
    ApplicationResumeEvent resume;
    ApplicationLowMemoryEvent low_memory;

    EXPECT_EQ(pause.type(), EventType::eApplicationPause);
    EXPECT_EQ(resume.type(), EventType::eApplicationResume);
    EXPECT_EQ(low_memory.type(), EventType::eApplicationLowMemory);

    EXPECT_EQ(pause.name(), "ApplicationPause");
    EXPECT_EQ(resume.name(), "ApplicationResume");
    EXPECT_EQ(low_memory.name(), "ApplicationLowMemory");
}

TEST(ApplicationEventTest, AreApplicationScopedNotWindowScoped) {
    ApplicationPauseEvent event;

    EXPECT_TRUE(event.isInCategory(EventCategory::eApplication));
    EXPECT_FALSE(event.isInCategory(EventCategory::eWindow));
    EXPECT_FALSE(event.isInCategory(EventCategory::eInput));
    EXPECT_EQ(event.windowId(), kInvalidWindowId);
}

// ============================================================================
// Text Input Event Tests
// ============================================================================

TEST(TextInputEventTest, CarriesUtf8Text) {
    TextInputEvent event("hi");

    EXPECT_EQ(event.type(), EventType::eTextInput);
    EXPECT_EQ(event.text(), "hi");
    EXPECT_EQ(event.name(), "TextInput");
    EXPECT_TRUE(event.isInCategory(EventCategory::eKeyboard));
    EXPECT_TRUE(event.isInCategory(EventCategory::eInput));
}

TEST(TextInputEventTest, HoldsMultiByteAndMultiCodePointCommits) {
    // IME commits and emoji arrive as one event with several code points.
    TextInputEvent event("\xE3\x81\x82\xE3\x81\x84");

    EXPECT_EQ(event.text().size(), 6U);
    EXPECT_NE(event.toString().find("TextInputEvent"), std::string::npos);
}

// ============================================================================
// UTF-8 Code Point Encoding Tests
// ============================================================================

TEST(Utf8FromCodePointTest, EncodesEachLength) {
    EXPECT_EQ(utf8FromCodePoint(U'A'), "A");                          // 1 byte
    EXPECT_EQ(utf8FromCodePoint(U'é'), "\xC3\xA9");                   // 2 bytes, e-acute
    EXPECT_EQ(utf8FromCodePoint(U'あ'), "\xE3\x81\x82");              // 3 bytes, hiragana A
    EXPECT_EQ(utf8FromCodePoint(U'\U0001F600'), "\xF0\x9F\x98\x80");  // 4 bytes, grinning face
}

TEST(Utf8FromCodePointTest, SurvivesTheRangeTheOldKeyCodePathDropped) {
    // KeyTypedEvent stored a KeyCode (int16_t), so any code point above 0x7FFF went negative and
    // was silently discarded downstream. These are the cases that regressed.
    const std::string cjk = utf8FromCodePoint(static_cast<char32_t>(0x8000));
    EXPECT_EQ(cjk.size(), 3U);
    EXPECT_EQ(TextInputEvent(cjk).text(), cjk);

    const std::string emoji = utf8FromCodePoint(U'\U0001F600');
    EXPECT_EQ(emoji.size(), 4U);
    EXPECT_EQ(TextInputEvent(emoji).text(), emoji);
}

TEST(Utf8FromCodePointTest, RejectsSurrogatesAndOutOfRange) {
    EXPECT_TRUE(utf8FromCodePoint(static_cast<char32_t>(0xD800)).empty());
    EXPECT_TRUE(utf8FromCodePoint(static_cast<char32_t>(0xDFFF)).empty());
    EXPECT_TRUE(utf8FromCodePoint(static_cast<char32_t>(0x110000)).empty());
}

// ============================================================================
// Touch Modifier Tests
// ============================================================================

TEST(TouchModifierTest, DefaultsToNone) {
    TouchPressEvent event(1U, 10.0, 20.0);

    EXPECT_EQ(event.modifiers(), 0);
}

TEST(TouchModifierTest, CarriesModifiers) {
    // iPadOS with a hardware keyboard, Android with a BT keyboard, touchscreen laptops.
    TouchPressEvent press(1U, 10.0, 20.0, ModifierKey::eModCtrl);
    TouchMoveEvent move(1U, 11.0, 21.0, ModifierKey::eModShift);
    TouchReleaseEvent release(1U, 12.0, 22.0, ModifierKey::eModCtrl | ModifierKey::eModShift);

    EXPECT_EQ(press.modifiers(), ModifierKey::eModCtrl);
    EXPECT_EQ(move.modifiers(), ModifierKey::eModShift);
    EXPECT_EQ(release.modifiers(), ModifierKey::eModCtrl | ModifierKey::eModShift);
}

// ============================================================================
// MouseButton::eUnknown Tests
// ============================================================================

TEST(MouseButtonTest, UnknownIsDistinctAndPreservesCanonicalValues) {
    // vneinteraction static_asserts on these three values; eUnknown must not disturb them.
    EXPECT_EQ(static_cast<int>(MouseButton::eLeft), 0);
    EXPECT_EQ(static_cast<int>(MouseButton::eRight), 1);
    EXPECT_EQ(static_cast<int>(MouseButton::eMiddle), 2);
    EXPECT_EQ(static_cast<int>(MouseButton::eUnknown), 0xFF);

    MouseButtonPressedEvent event(MouseButton::eUnknown);
    EXPECT_EQ(event.button(), MouseButton::eUnknown);
}

// ============================================================================
// Mouse Scroll Position Tests
// ============================================================================

TEST(MouseScrollTest, HasNoPositionByDefault) {
    MouseScrolledEvent event(0.0, 1.0);

    EXPECT_FALSE(event.hasPosition());
    EXPECT_EQ(event.modifiers(), 0);
    EXPECT_EQ(event.toString(), "MouseScrolledEvent: (0, 1)");
}

TEST(MouseScrollTest, CarriesCursorPositionAndModifiers) {
    MouseScrolledEvent event(0.0, -3.0, 640.0, 480.0, ModifierKey::eModCtrl);

    EXPECT_TRUE(event.hasPosition());
    EXPECT_DOUBLE_EQ(event.x(), 640.0);
    EXPECT_DOUBLE_EQ(event.y(), 480.0);
    EXPECT_EQ(event.modifiers(), ModifierKey::eModCtrl);
    EXPECT_NE(event.toString().find("at (640, 480)"), std::string::npos);
}

// ============================================================================
// Key Repeat Modifier Tests
// ============================================================================

TEST(KeyRepeatTest, CarriesModifiers) {
    KeyRepeatEvent event(KeyCode::eA, 4U, ModifierKey::eModShift);

    EXPECT_EQ(event.repeatCount(), 4U);
    EXPECT_EQ(event.modifiers(), ModifierKey::eModShift);
}

}  // namespace vne::events
