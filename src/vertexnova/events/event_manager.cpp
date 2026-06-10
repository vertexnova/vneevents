/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   June 2026
 *
 * Autodoc:   yes
 * ----------------------------------------------------------------------
 */

#include "vertexnova/events/event_manager.h"

namespace vne::events {

EventManager& EventManager::instance() {
    static EventManager s_event_manager_instance;
    return s_event_manager_instance;
}

void EventManager::registerListener(EventType event_type, ListenerPtr listener) {
    event_bus_.registerListener(event_type, std::move(listener));
}

void EventManager::unregisterListener(EventType event_type, const EventListener* listener) {
    event_bus_.unregisterListener(event_type, listener);
}

void EventManager::pushEvent(std::unique_ptr<Event> event) {
    event_queue_.push(std::move(event));
}

void EventManager::processEvents() {
    event_bus_.processEvents(event_queue_);
}

void EventManager::dispatchImmediate(const Event& event) {
    event_bus_.dispatchImmediate(event);
}

size_t EventManager::pendingEventCount() const {
    return event_queue_.size();
}

void EventManager::clearPendingEvents() {
    event_queue_.clear();
}

}  // namespace vne::events
