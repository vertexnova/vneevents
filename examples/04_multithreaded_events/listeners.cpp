/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener implementations for Multithreaded Events example
 * ----------------------------------------------------------------------
 */

#include "listeners.h"

#include <vertexnova/logging/logging.h>

namespace  {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.multithreaded_events")
}

namespace vne::events::examples {

EventCounter::EventCounter(std::atomic<int>* counter)
    : counter_(counter) {}

void EventCounter::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        (*counter_)++;
    }
}

ThreadListener::ThreadListener(int thread_id)
    : thread_id_(thread_id) {}

void ThreadListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        VNE_LOG_INFO << "    [Thread " << thread_id_ << "] Received key event";
    }
}

SafeListener::SafeListener(std::atomic<int>* counter)
    : counter_(counter) {}

void SafeListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eMouseMoved) {
        (*counter_)++;
    }
}

}  // namespace vne::events::examples
