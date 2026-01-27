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

#include <sstream>
#include <cstdint>

namespace vne::events {

/**
 * @class MouseButtonEvent
 * @brief Base class for mouse button events.
 */
class MouseButtonEvent : public Event {
   public:
    [[nodiscard]] MouseButton button() const noexcept { return button_; }
    [[nodiscard]] uint8_t modifiers() const noexcept { return modifiers_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eMouseButton | EventCategory::eInput; }

   protected:
    MouseButtonEvent(EventType type, MouseButton button, uint8_t modifiers = 0)
        : Event(type)
        , button_(button)
        , modifiers_(modifiers) {}

   private:
    MouseButton button_;
    uint8_t modifiers_;
};

/**
 * @class MouseButtonPressedEvent
 * @brief Event generated when a mouse button is pressed.
 */
class MouseButtonPressedEvent : public MouseButtonEvent {
   public:
    explicit MouseButtonPressedEvent(MouseButton button, uint8_t modifiers = 0)
        : MouseButtonEvent(EventType::eMouseButtonPressed, button, modifiers) {}

    [[nodiscard]] std::string name() const override { return "MouseButtonPressed"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonPressedEvent: " << static_cast<int>(button());
        return ss.str();
    }
};

/**
 * @class MouseButtonReleasedEvent
 * @brief Event generated when a mouse button is released.
 */
class MouseButtonReleasedEvent : public MouseButtonEvent {
   public:
    explicit MouseButtonReleasedEvent(MouseButton button, uint8_t modifiers = 0)
        : MouseButtonEvent(EventType::eMouseButtonReleased, button, modifiers) {}

    [[nodiscard]] std::string name() const override { return "MouseButtonReleased"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonReleasedEvent: " << static_cast<int>(button());
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
