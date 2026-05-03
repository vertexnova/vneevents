#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener interface for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "export.h"

namespace vne::events {

// Forward declaration
class Event;

/**
 * @class EventListener
 * @brief Interface class for event listeners.
 *
 * Classes implementing EventListener can subscribe to specific event types
 * and define custom behavior to handle those events.
 */
class VNEEVENTS_API EventListener {
   public:
    virtual ~EventListener() = default;

    /**
     * @brief Callback method invoked when an event occurs.
     * @param event Reference to the event object that occurred.
     *
     * Implementing classes must override this method to define their
     * specific behavior in response to the event.
     */
    virtual void onEvent(const Event& event) = 0;
};

}  // namespace vne::events
