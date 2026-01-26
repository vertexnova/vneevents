#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Base event interface for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "types.h"

#include <string>
#include <ostream>

namespace vne::events {

/**
 * @class Event
 * @brief Base class for all events in the VertexNova event system.
 *
 * This abstract class defines the common interface for all events.
 * Events have a type, timestamp, category flags, and can be marked as handled.
 */
class Event {
   public:
    virtual ~Event() = default;

    /// Get the category flags for the event
    [[nodiscard]] virtual int categoryFlags() const = 0;

    /// Get the name of the event
    [[nodiscard]] virtual std::string name() const = 0;

    /// Get a string representation of the event
    [[nodiscard]] virtual std::string toString() const { return name(); }

    /// Get the event type
    [[nodiscard]] EventType type() const noexcept { return type_; }

    /// Get the timestamp of when the event occurred
    [[nodiscard]] TimeStamp timestamp() const noexcept { return timestamp_; }

    /// Check if the event has been handled
    [[nodiscard]] bool isHandled() const noexcept { return handled_; }

    /// Mark the event as handled
    void setHandled() noexcept { handled_ = true; }

    /// Check if a category flag exists in the event
    [[nodiscard]] bool isInCategory(EventCategory category) const noexcept { return (categoryFlags() & category) != 0; }

   protected:
    explicit Event(EventType event_type) noexcept
        : type_(event_type)
        , timestamp_(std::chrono::system_clock::now())
        , handled_(false) {}

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

   private:
    EventType type_;
    TimeStamp timestamp_;
    bool handled_;
};

/// Output stream operator for Event
inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    return os << event.toString();
}

}  // namespace vne::events
