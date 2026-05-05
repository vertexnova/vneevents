#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Window event classes for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "types.h"

#include <sstream>
#include <cstdint>

namespace vne::events {

/**
 * @class WindowCloseEvent
 * @brief Event generated when a window is closed.
 */
class VNEEVENTS_API WindowCloseEvent : public Event {
   public:
    WindowCloseEvent()
        : Event(EventType::eWindowClose) {}

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eWindow; }

    [[nodiscard]] std::string name() const override { return "WindowClose"; }

    [[nodiscard]] std::string toString() const override { return "WindowCloseEvent"; }
};

/**
 * @class WindowResizeEvent
 * @brief Event generated when a window is resized.
 */
class VNEEVENTS_API WindowResizeEvent : public Event {
   public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : Event(EventType::eWindowResize)
        , width_(width)
        , height_(height) {}

    [[nodiscard]] uint32_t width() const noexcept { return width_; }
    [[nodiscard]] uint32_t height() const noexcept { return height_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eWindow; }

    [[nodiscard]] std::string name() const override { return "WindowResize"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "WindowResizeEvent: (" << width_ << ", " << height_ << ")";
        return ss.str();
    }

   private:
    uint32_t width_;
    uint32_t height_;
};

/**
 * @class WindowFocusEvent
 * @brief Event generated when a window gains or loses focus.
 */
class WindowFocusEvent : public Event {
   public:
    explicit WindowFocusEvent(bool focused)
        : Event(EventType::eWindowFocus)
        , focused_(focused) {}

    [[nodiscard]] bool focused() const noexcept { return focused_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eWindow; }

    [[nodiscard]] std::string name() const override { return "WindowFocus"; }

    [[nodiscard]] std::string toString() const override {
        return focused_ ? "WindowFocusEvent: gained" : "WindowFocusEvent: lost";
    }

   private:
    bool focused_;
};

}  // namespace vne::events
