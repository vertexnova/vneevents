#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   August 2026
 *
 * Application lifecycle event classes for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "types.h"

#include <string>

namespace vne::events {

/**
 * @class ApplicationEvent
 * @brief Base class for process-scoped application lifecycle events.
 *
 * These describe the state of the application as a whole rather than of any one window, so they
 * carry kInvalidWindowId. On mobile they map to the platform lifecycle callbacks (UIKit scene /
 * application notifications, Android APP_CMD_*); on desktop and web they map to the nearest
 * equivalent (window occlusion, page visibility).
 */
class VNEEVENTS_API ApplicationEvent : public Event {
   public:
    [[nodiscard]] int categoryFlags() const override { return EventCategory::eApplication; }

   protected:
    explicit ApplicationEvent(EventType event_type) noexcept
        : Event(event_type, kInvalidWindowId) {}
};

/**
 * @class ApplicationPauseEvent
 * @brief The application is no longer active and will stop receiving frames.
 *
 * Emitted when the app moves to the background or otherwise resigns active status. Renderers
 * should stop presenting and may release transient GPU resources; the drawable surface can
 * become invalid at any point after this event. Always paired with a later ApplicationResumeEvent
 * unless the process is terminated.
 */
class VNEEVENTS_API ApplicationPauseEvent : public ApplicationEvent {
   public:
    ApplicationPauseEvent()
        : ApplicationEvent(EventType::eApplicationPause) {}

    [[nodiscard]] std::string name() const override { return "ApplicationPause"; }

    [[nodiscard]] std::string toString() const override { return "ApplicationPauseEvent"; }
};

/**
 * @class ApplicationResumeEvent
 * @brief The application became active again and will resume receiving frames.
 *
 * Consumers must treat any surface or swapchain acquired before the matching ApplicationPauseEvent as
 * stale and re-query window size and DPI, which may have changed while backgrounded.
 */
class VNEEVENTS_API ApplicationResumeEvent : public ApplicationEvent {
   public:
    ApplicationResumeEvent()
        : ApplicationEvent(EventType::eApplicationResume) {}

    [[nodiscard]] std::string name() const override { return "ApplicationResume"; }

    [[nodiscard]] std::string toString() const override { return "ApplicationResumeEvent"; }
};

/**
 * @class ApplicationLowMemoryEvent
 * @brief The system is under memory pressure and asks the application to free what it can.
 *
 * Maps to UIKit's didReceiveMemoryWarning and Android's APP_CMD_LOW_MEMORY. Ignoring it on
 * mobile invites termination by the OS.
 */
class VNEEVENTS_API ApplicationLowMemoryEvent : public ApplicationEvent {
   public:
    ApplicationLowMemoryEvent()
        : ApplicationEvent(EventType::eApplicationLowMemory) {}

    [[nodiscard]] std::string name() const override { return "ApplicationLowMemory"; }

    [[nodiscard]] std::string toString() const override { return "ApplicationLowMemoryEvent"; }
};

}  // namespace vne::events
