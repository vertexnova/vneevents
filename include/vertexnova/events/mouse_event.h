#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Mouse event classes for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "types.h"

#include <cmath>
#include <limits>
#include <sstream>
#include <cstdint>

namespace vne::events {

/**
 * @class MouseButtonEvent
 * @brief Base class for mouse button events.
 *
 * Cursor position (x(), y()) is in **window/client coordinates**: origin at the
 * top-left of the window content area, units in pixels. This matches common
 * windowing APIs (e.g. GLFW content-area coordinates).
 *
 * When position is not available (e.g. synthetic events or backends that do not
 * report it), x() and y() return NaN. Use hasPosition() before using coordinates;
 * toString() omits the "at (x, y)" suffix when position was not supplied.
 */
class MouseButtonEvent : public Event {
   public:
    [[nodiscard]] MouseButton button() const noexcept { return button_; }
    [[nodiscard]] uint8_t modifiers() const noexcept { return modifiers_; }
    /** Cursor x in window/client coords (pixels); NaN if not provided. */
    [[nodiscard]] double x() const noexcept { return x_; }
    /** Cursor y in window/client coords (pixels); NaN if not provided. */
    [[nodiscard]] double y() const noexcept { return y_; }
    /** True if a real cursor position was supplied (x and y are not NaN). */
    [[nodiscard]] bool hasPosition() const noexcept {
        return !std::isnan(x_) && !std::isnan(y_);
    }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eMouseButton | EventCategory::eInput; }

   protected:
    /** @param x Cursor x (pixels); NaN when not available. @param y Cursor y (pixels); NaN when not available. */
    MouseButtonEvent(EventType type, MouseButton button, uint8_t modifiers = 0,
                    double x = std::numeric_limits<double>::quiet_NaN(),
                    double y = std::numeric_limits<double>::quiet_NaN())
        : Event(type)
        , button_(button)
        , modifiers_(modifiers)
        , x_(x)
        , y_(y) {}

   private:
    MouseButton button_;
    uint8_t modifiers_;
    double x_;
    double y_;
};

/**
 * @class MouseButtonPressedEvent
 * @brief Event generated when a mouse button is pressed.
 *
 * Position semantics: see MouseButtonEvent (NaN when not provided; use hasPosition()).
 */
class MouseButtonPressedEvent : public MouseButtonEvent {
   public:
    /**
     * @param x Cursor x in window/client coords (pixels); NaN if not available.
     * @param y Cursor y in window/client coords (pixels); NaN if not available.
     */
    explicit MouseButtonPressedEvent(MouseButton button, uint8_t modifiers = 0,
                                    double x = std::numeric_limits<double>::quiet_NaN(),
                                    double y = std::numeric_limits<double>::quiet_NaN())
        : MouseButtonEvent(EventType::eMouseButtonPressed, button, modifiers, x, y) {}

    [[nodiscard]] std::string name() const override { return "MouseButtonPressed"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonPressedEvent: " << static_cast<int>(button());
        if (hasPosition()) { ss << " at (" << x() << ", " << y() << ")"; }
        return ss.str();
    }
};

/**
 * @class MouseButtonReleasedEvent
 * @brief Event generated when a mouse button is released.
 *
 * Position semantics: see MouseButtonEvent (NaN when not provided; use hasPosition()).
 */
class MouseButtonReleasedEvent : public MouseButtonEvent {
   public:
    /**
     * @param x Cursor x in window/client coords (pixels); NaN if not available.
     * @param y Cursor y in window/client coords (pixels); NaN if not available.
     */
    explicit MouseButtonReleasedEvent(MouseButton button, uint8_t modifiers = 0,
                                     double x = std::numeric_limits<double>::quiet_NaN(),
                                     double y = std::numeric_limits<double>::quiet_NaN())
        : MouseButtonEvent(EventType::eMouseButtonReleased, button, modifiers, x, y) {}

    [[nodiscard]] std::string name() const override { return "MouseButtonReleased"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonReleasedEvent: " << static_cast<int>(button());
        if (hasPosition()) { ss << " at (" << x() << ", " << y() << ")"; }
        return ss.str();
    }
};

/**
 * @class MouseButtonDoubleClickedEvent
 * @brief Event generated when a mouse button is double-clicked.
 *
 * Position semantics: see MouseButtonEvent (NaN when not provided; use hasPosition()).
 */
class MouseButtonDoubleClickedEvent : public MouseButtonEvent {
   public:
    /**
     * @param x Cursor x in window/client coords (pixels); NaN if not available.
     * @param y Cursor y in window/client coords (pixels); NaN if not available.
     */
    explicit MouseButtonDoubleClickedEvent(MouseButton button, uint8_t modifiers = 0,
                                           double x = std::numeric_limits<double>::quiet_NaN(),
                                           double y = std::numeric_limits<double>::quiet_NaN())
        : MouseButtonEvent(EventType::eMouseButtonDoubleClicked, button, modifiers, x, y) {}

    [[nodiscard]] std::string name() const override { return "MouseButtonDoubleClicked"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonDoubleClickedEvent: " << static_cast<int>(button());
        if (hasPosition()) { ss << " at (" << x() << ", " << y() << ")"; }
        return ss.str();
    }
};

/**
 * @class MouseMovedEvent
 * @brief Event generated when the mouse cursor moves.
 */
class MouseMovedEvent : public Event {
   public:
    MouseMovedEvent(double x_pos, double y_pos, uint8_t modifiers = 0)
        : Event(EventType::eMouseMoved)
        , x_(x_pos)
        , y_(y_pos)
        , modifiers_(modifiers) {}

    [[nodiscard]] double x() const noexcept { return x_; }
    [[nodiscard]] double y() const noexcept { return y_; }
    [[nodiscard]] uint8_t modifiers() const noexcept { return modifiers_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eMouse | EventCategory::eInput; }

    [[nodiscard]] std::string name() const override { return "MouseMoved"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseMovedEvent: (" << x_ << ", " << y_ << ")";
        return ss.str();
    }

   private:
    double x_;
    double y_;
    uint8_t modifiers_;
};

/**
 * @class MouseScrolledEvent
 * @brief Event generated when the mouse wheel is scrolled.
 */
class MouseScrolledEvent : public Event {
   public:
    MouseScrolledEvent(double x_offset, double y_offset)
        : Event(EventType::eMouseScrolled)
        , x_offset_(x_offset)
        , y_offset_(y_offset) {}

    [[nodiscard]] double xOffset() const noexcept { return x_offset_; }
    [[nodiscard]] double yOffset() const noexcept { return y_offset_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eMouse | EventCategory::eInput; }

    [[nodiscard]] std::string name() const override { return "MouseScrolled"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseScrolledEvent: (" << x_offset_ << ", " << y_offset_ << ")";
        return ss.str();
    }

   private:
    double x_offset_;
    double y_offset_;
};

}  // namespace vne::events
