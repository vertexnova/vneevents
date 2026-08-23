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
 * @class WindowEvent
 * @brief Base class for events describing the state of a single window.
 *
 * Supplies EventCategory::eWindow once for the whole family. Every window event carries a
 * WindowId identifying which window it came from; producers that manage a single window may
 * leave it as kInvalidWindowId.
 */
class VNEEVENTS_API WindowEvent : public Event {
   public:
    [[nodiscard]] int categoryFlags() const override { return EventCategory::eWindow; }

   protected:
    explicit WindowEvent(EventType event_type, WindowId window_id = kInvalidWindowId) noexcept
        : Event(event_type, window_id) {}
};

/**
 * @class WindowCloseEvent
 * @brief Event generated when a window is closed.
 */
class VNEEVENTS_API WindowCloseEvent : public WindowEvent {
   public:
    explicit WindowCloseEvent(WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowClose, window_id) {}

    [[nodiscard]] std::string name() const override { return "WindowClose"; }

    [[nodiscard]] std::string toString() const override { return "WindowCloseEvent" + windowSuffix(); }
};

/**
 * @class WindowResizeEvent
 * @brief Event generated when a window is resized.
 *
 * Dimensions are in logical (client-area) pixels. Combine with WindowDpiChangedEvent::scale()
 * or the window's own DPI query to derive framebuffer dimensions.
 */
class VNEEVENTS_API WindowResizeEvent : public WindowEvent {
   public:
    WindowResizeEvent(uint32_t width, uint32_t height, WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowResize, window_id)
        , width_(width)
        , height_(height) {}

    [[nodiscard]] uint32_t width() const noexcept { return width_; }
    [[nodiscard]] uint32_t height() const noexcept { return height_; }

    [[nodiscard]] std::string name() const override { return "WindowResize"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "WindowResizeEvent: (" << width_ << ", " << height_ << ")" << windowSuffix();
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
class VNEEVENTS_API WindowFocusEvent : public WindowEvent {
   public:
    explicit WindowFocusEvent(bool focused, WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowFocus, window_id)
        , focused_(focused) {}

    [[nodiscard]] bool focused() const noexcept { return focused_; }

    [[nodiscard]] std::string name() const override { return "WindowFocus"; }

    [[nodiscard]] std::string toString() const override {
        return (focused_ ? "WindowFocusEvent: gained" : "WindowFocusEvent: lost") + windowSuffix();
    }

   private:
    bool focused_;
};

/**
 * @class WindowMinimizeEvent
 * @brief Event generated when a window is minimized / iconified / moved to the background.
 *
 * Renderers typically use this to stop presenting: the swapchain surface may have zero extent
 * or be unavailable until the matching WindowRestoreEvent arrives.
 */
class VNEEVENTS_API WindowMinimizeEvent : public WindowEvent {
   public:
    explicit WindowMinimizeEvent(WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowMinimize, window_id) {}

    [[nodiscard]] std::string name() const override { return "WindowMinimize"; }

    [[nodiscard]] std::string toString() const override { return "WindowMinimizeEvent" + windowSuffix(); }
};

/**
 * @class WindowRestoreEvent
 * @brief Event generated when a window is restored from a minimized or maximized state.
 *
 * A WindowResizeEvent may or may not accompany this; consumers should re-query the window size.
 */
class VNEEVENTS_API WindowRestoreEvent : public WindowEvent {
   public:
    explicit WindowRestoreEvent(WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowRestore, window_id) {}

    [[nodiscard]] std::string name() const override { return "WindowRestore"; }

    [[nodiscard]] std::string toString() const override { return "WindowRestoreEvent" + windowSuffix(); }
};

/**
 * @class WindowMoveEvent
 * @brief Event generated when a window's top-left position changes, in screen coordinates.
 *
 * Not emitted by platforms that do not report window position (e.g. Wayland, mobile, web).
 */
class VNEEVENTS_API WindowMoveEvent : public WindowEvent {
   public:
    WindowMoveEvent(int32_t x, int32_t y, WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowMove, window_id)
        , x_(x)
        , y_(y) {}

    [[nodiscard]] int32_t x() const noexcept { return x_; }
    [[nodiscard]] int32_t y() const noexcept { return y_; }

    [[nodiscard]] std::string name() const override { return "WindowMove"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "WindowMoveEvent: (" << x_ << ", " << y_ << ")" << windowSuffix();
        return ss.str();
    }

   private:
    int32_t x_;
    int32_t y_;
};

/**
 * @class WindowDpiChangedEvent
 * @brief Event generated when a window's backing scale factor changes.
 *
 * Fires when the window moves between displays of different density, when the OS display scale
 * changes, or on mobile when the trait environment changes. scale() is the ratio of physical
 * (framebuffer) pixels to logical pixels, e.g. 2.0 on a typical Retina display.
 */
class VNEEVENTS_API WindowDpiChangedEvent : public WindowEvent {
   public:
    explicit WindowDpiChangedEvent(float scale, WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowDpiChanged, window_id)
        , scale_(scale) {}

    [[nodiscard]] float scale() const noexcept { return scale_; }

    [[nodiscard]] std::string name() const override { return "WindowDpiChanged"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "WindowDpiChangedEvent: " << scale_ << windowSuffix();
        return ss.str();
    }

   private:
    float scale_;
};

/**
 * @class WindowSafeAreaChangedEvent
 * @brief Event generated when the window's safe-area insets change.
 *
 * Insets are in logical pixels, measured inward from each edge of the window. They describe the
 * region obscured by system UI (notch, dynamic island, home indicator, status bar, rounded
 * corners). Zero on platforms without system overlays.
 */
class VNEEVENTS_API WindowSafeAreaChangedEvent : public WindowEvent {
   public:
    WindowSafeAreaChangedEvent(float top, float left, float bottom, float right, WindowId window_id = kInvalidWindowId)
        : WindowEvent(EventType::eWindowSafeAreaChanged, window_id)
        , top_(top)
        , left_(left)
        , bottom_(bottom)
        , right_(right) {}

    [[nodiscard]] float top() const noexcept { return top_; }
    [[nodiscard]] float left() const noexcept { return left_; }
    [[nodiscard]] float bottom() const noexcept { return bottom_; }
    [[nodiscard]] float right() const noexcept { return right_; }

    [[nodiscard]] std::string name() const override { return "WindowSafeAreaChanged"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "WindowSafeAreaChangedEvent: top=" << top_ << " left=" << left_ << " bottom=" << bottom_
           << " right=" << right_ << windowSuffix();
        return ss.str();
    }

   private:
    float top_;
    float left_;
    float bottom_;
    float right_;
};

}  // namespace vne::events
