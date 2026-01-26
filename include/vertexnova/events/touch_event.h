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
class TouchEvent : public Event {
   public:
    [[nodiscard]] uint32_t touchId() const noexcept { return touch_id_; }
    [[nodiscard]] double x() const noexcept { return x_; }
    [[nodiscard]] double y() const noexcept { return y_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eTouchScreen | EventCategory::eInput; }

   protected:
    TouchEvent(EventType type, uint32_t touch_id, double x, double y)
        : Event(type)
        , touch_id_(touch_id)
        , x_(x)
        , y_(y) {}

   private:
    uint32_t touch_id_;
    double x_;
    double y_;
};

/**
 * @class TouchPressEvent
 * @brief Event generated when a touch begins.
 */
class TouchPressEvent : public TouchEvent {
   public:
    TouchPressEvent(uint32_t touch_id, double x, double y)
        : TouchEvent(EventType::eTouchPress, touch_id, x, y) {}

    [[nodiscard]] std::string name() const override { return "TouchPress"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "TouchPressEvent: id=" << touchId() << " (" << x() << ", " << y() << ")";
        return ss.str();
    }
};

/**
 * @class TouchReleaseEvent
 * @brief Event generated when a touch ends.
 */
class TouchReleaseEvent : public TouchEvent {
   public:
    TouchReleaseEvent(uint32_t touch_id, double x, double y)
        : TouchEvent(EventType::eTouchRelease, touch_id, x, y) {}

    [[nodiscard]] std::string name() const override { return "TouchRelease"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "TouchReleaseEvent: id=" << touchId() << " (" << x() << ", " << y() << ")";
        return ss.str();
    }
};

/**
 * @class TouchMoveEvent
 * @brief Event generated when a touch moves.
 */
class TouchMoveEvent : public TouchEvent {
   public:
    TouchMoveEvent(uint32_t touch_id, double x, double y)
        : TouchEvent(EventType::eTouchMove, touch_id, x, y) {}

    [[nodiscard]] std::string name() const override { return "TouchMove"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "TouchMoveEvent: id=" << touchId() << " (" << x() << ", " << y() << ")";
        return ss.str();
    }
};

}  // namespace vne::events
