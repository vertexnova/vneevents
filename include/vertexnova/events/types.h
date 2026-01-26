#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event types, key codes, and input definitions for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include <chrono>
#include <functional>
#include <cstdint>

namespace vne::events {

/// Number of key states to track
inline constexpr int kNumKeyStates = 380;

/// Number of mouse button states to track
inline constexpr int kNumMouseButtonStates = 12;

/// Bit manipulation helper
template<typename T>
constexpr T bit(T x) noexcept {
    return static_cast<T>(1) << x;
}

/// Timestamp type for events
using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;

/**
 * @brief Callback function type for event handlers.
 */
template<typename T>
using EventCallback = std::function<bool(T&)>;

/**
 * @enum EventType
 * @brief Describes the input event types.
 */
enum class EventType : uint8_t {
    eInvalid = 0,
    eWindowClose = 1,
    eWindowResize = 2,
    eKeyPressed = 3,
    eKeyReleased = 4,
    eKeyRepeat = 5,
    eKeyTyped = 6,
    eMouseButtonPressed = 7,
    eMouseButtonReleased = 8,
    eMouseMoved = 9,
    eMouseScrolled = 10,
    eTouchPress = 11,
    eTouchRelease = 12,
    eTouchMove = 13
};

/**
 * @enum EventCategory
 * @brief Bit flags for filtering event categories.
 */
enum EventCategory : uint8_t {
    eNone = 0,
    eInput = bit<uint8_t>(0),
    eKeyboard = bit<uint8_t>(1),
    eMouse = bit<uint8_t>(2),
    eMouseButton = bit<uint8_t>(3),
    eTouchScreen = bit<uint8_t>(4),
    eWindow = bit<uint8_t>(5)
};

/**
 * @enum MouseButton
 * @brief Mouse button identifiers.
 */
enum class MouseButton : uint8_t {
    eButton0 = 0,
    eButton1 = 1,
    eButton2 = 2,
    eButton3 = 3,
    eButton4 = 4,
    eButton5 = 5,
    eButton6 = 6,
    eButton7 = 7,
    eLeft = eButton0,
    eRight = eButton1,
    eMiddle = eButton2,
    eLast = eButton7
};

/**
 * @enum ButtonState
 * @brief Button state (pressed/released).
 */
enum class ButtonState : uint8_t { ePressed = 0, eReleased = 1 };

/**
 * @enum ModifierKey
 * @brief Keyboard modifier key flags.
 */
enum ModifierKey : uint8_t {
    eModNone = 0,
    eModShift = bit<uint8_t>(0),
    eModCtrl = bit<uint8_t>(1),
    eModAlt = bit<uint8_t>(2),
    eModMeta = bit<uint8_t>(3),
    eModSuper = bit<uint8_t>(4),
    eModFn = bit<uint8_t>(5),
    eModAltGr = bit<uint8_t>(6),
    eModCmd = bit<uint8_t>(7)
};

/**
 * @enum KeyCode
 * @brief Keyboard key codes (based on USB HID Usage Tables).
 *
 * The naming follows the US keyboard layout. For non-alphanumeric
 * characters, Unicode-ish names are used.
 */
enum class KeyCode : int16_t {
    eUnknown = -1,
    eSpace = 32,
    eApostrophe = 39,
    eComma = 44,
    eMinus = 45,
    ePeriod = 46,
    eSlash = 47,
    e0 = 48,
    e1 = 49,
    e2 = 50,
    e3 = 51,
    e4 = 52,
    e5 = 53,
    e6 = 54,
    e7 = 55,
    e8 = 56,
    e9 = 57,
    eSemicolon = 59,
    eEqual = 61,
    eA = 65,
    eB = 66,
    eC = 67,
    eD = 68,
    eE = 69,
    eF = 70,
    eG = 71,
    eH = 72,
    eI = 73,
    eJ = 74,
    eK = 75,
    eL = 76,
    eM = 77,
    eN = 78,
    eO = 79,
    eP = 80,
    eQ = 81,
    eR = 82,
    eS = 83,
    eT = 84,
    eU = 85,
    eV = 86,
    eW = 87,
    eX = 88,
    eY = 89,
    eZ = 90,
    eLeftBracket = 91,
    eBackslash = 92,
    eRightBracket = 93,
    eGraveAccent = 96,
    eWorld1 = 161,
    eWorld2 = 162,
    // Function keys
    eEscape = 256,
    eEnter = 257,
    eTab = 258,
    eBackspace = 259,
    eInsert = 260,
    eDelete = 261,
    eRight = 262,
    eLeft = 263,
    eDown = 264,
    eUp = 265,
    ePageUp = 266,
    ePageDown = 267,
    eHome = 268,
    eEnd = 269,
    eCapsLock = 280,
    eScrollLock = 281,
    eNumLock = 282,
    ePrintScreen = 283,
    ePause = 284,
    eF1 = 290,
    eF2 = 291,
    eF3 = 292,
    eF4 = 293,
    eF5 = 294,
    eF6 = 295,
    eF7 = 296,
    eF8 = 297,
    eF9 = 298,
    eF10 = 299,
    eF11 = 300,
    eF12 = 301,
    eF13 = 302,
    eF14 = 303,
    eF15 = 304,
    eF16 = 305,
    eF17 = 306,
    eF18 = 307,
    eF19 = 308,
    eF20 = 309,
    eF21 = 310,
    eF22 = 311,
    eF23 = 312,
    eF24 = 313,
    eF25 = 314,
    // Keypad
    eKp0 = 320,
    eKp1 = 321,
    eKp2 = 322,
    eKp3 = 323,
    eKp4 = 324,
    eKp5 = 325,
    eKp6 = 326,
    eKp7 = 327,
    eKp8 = 328,
    eKp9 = 329,
    eKpDecimal = 330,
    eKpDivide = 331,
    eKpMultiply = 332,
    eKpSubtract = 333,
    eKpAdd = 334,
    eKpEnter = 335,
    eKpEqual = 336,
    // Modifier keys
    eLeftShift = 340,
    eLeftControl = 341,
    eLeftAlt = 342,
    eLeftSuper = 343,
    eRightShift = 344,
    eRightControl = 345,
    eRightAlt = 346,
    eRightSuper = 347,
    eMenu = 348,
    eLast = eMenu
};

}  // namespace vne::events
