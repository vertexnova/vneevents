#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Touch event classes for VertexNova Events (mobile platforms).
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "types.h"

#include <sstream>
#include <cstdint>

namespace vne::events {

/**
 * @class TouchEvent
 * @brief Base class for touch screen events.
 */
class VNEEVENTS_API TouchEvent : public Event {
   public:
    [[nodiscard]] uint32_t touchId() const noexcept { return touch_id_; }
    [[nodiscard]] double x() const noexcept { return x_; }
    [[nodiscard]] double y() const noexcept { return y_; }
    /**
     * @brief Modifier keys held during the touch (e.g. Shift, Ctrl).
     *
     * Non-zero only where a hardware keyboard can accompany touch — iPadOS with a Magic Keyboard,
     * Android with a Bluetooth keyboard, a touchscreen laptop, or a touch-capable browser. Zero on
     * platforms that report no modifier state.
     */
    [[nodiscard]] uint8_t modifiers() const noexcept { return modifiers_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eTouchScreen | EventCategory::eInput; }

   protected:
    TouchEvent(EventType type,
               uint32_t touch_id,
               double x,
               double y,
               uint8_t modifiers = 0,
               WindowId window_id = kInvalidWindowId)
        : Event(type, window_id)
        , touch_id_(touch_id)
        , x_(x)
        , y_(y)
        , modifiers_(modifiers) {}

   private:
    uint32_t touch_id_;
    double x_;
    double y_;
    uint8_t modifiers_;
};

/**
 * @class TouchPressEvent
 * @brief Event generated when a touch begins.
 */
class VNEEVENTS_API TouchPressEvent : public TouchEvent {
   public:
    TouchPressEvent(uint32_t touch_id, double x, double y, uint8_t modifiers = 0, WindowId window_id = kInvalidWindowId)
        : TouchEvent(EventType::eTouchPress, touch_id, x, y, modifiers, window_id) {}

    [[nodiscard]] std::string name() const override { return "TouchPress"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "TouchPressEvent: id=" << touchId() << " (" << x() << ", " << y() << ")" << windowSuffix();
        return ss.str();
    }
};

/**
 * @class TouchReleaseEvent
 * @brief Event generated when a touch ends.
 */
class VNEEVENTS_API TouchReleaseEvent : public TouchEvent {
   public:
    TouchReleaseEvent(
        uint32_t touch_id, double x, double y, uint8_t modifiers = 0, WindowId window_id = kInvalidWindowId)
        : TouchEvent(EventType::eTouchRelease, touch_id, x, y, modifiers, window_id) {}

    [[nodiscard]] std::string name() const override { return "TouchRelease"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "TouchReleaseEvent: id=" << touchId() << " (" << x() << ", " << y() << ")" << windowSuffix();
        return ss.str();
    }
};

/**
 * @class TouchMoveEvent
 * @brief Event generated when a touch moves.
 */
class VNEEVENTS_API TouchMoveEvent : public TouchEvent {
   public:
    TouchMoveEvent(uint32_t touch_id, double x, double y, uint8_t modifiers = 0, WindowId window_id = kInvalidWindowId)
        : TouchEvent(EventType::eTouchMove, touch_id, x, y, modifiers, window_id) {}

    [[nodiscard]] std::string name() const override { return "TouchMove"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "TouchMoveEvent: id=" << touchId() << " (" << x() << ", " << y() << ")" << windowSuffix();
        return ss.str();
    }
};

}  // namespace vne::events
