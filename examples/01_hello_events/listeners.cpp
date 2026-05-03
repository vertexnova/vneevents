/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener implementations for Hello Events example
 * ----------------------------------------------------------------------
 */

#include "listeners.h"

#include <vertexnova/logging/logging.h>

namespace {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.hello_events")
}
namespace vne::events::examples {

void KeyboardListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
        VNE_LOG_INFO << "  Received: " << key_event.toString();
    } else if (event.type() == vne::events::EventType::eKeyReleased) {
        const auto& key_event = static_cast<const vne::events::KeyReleasedEvent&>(event);
        VNE_LOG_INFO << "  Received: " << key_event.toString();
    }
}

void MouseListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eMouseMoved) {
        const auto& mouse_event = static_cast<const vne::events::MouseMovedEvent&>(event);
        VNE_LOG_INFO << "  Received: " << mouse_event.toString();
    } else if (event.type() == vne::events::EventType::eMouseButtonPressed) {
        const auto& button_event = static_cast<const vne::events::MouseButtonPressedEvent&>(event);
        VNE_LOG_INFO << "  Received: " << button_event.toString();
    } else if (event.type() == vne::events::EventType::eMouseButtonDoubleClicked) {
        const auto& button_event = static_cast<const vne::events::MouseButtonDoubleClickedEvent&>(event);
        VNE_LOG_INFO << "  Received: " << button_event.toString();
    } else if (event.type() == vne::events::EventType::eMouseScrolled) {
        const auto& scroll_event = static_cast<const vne::events::MouseScrolledEvent&>(event);
        VNE_LOG_INFO << "  Received: " << scroll_event.toString();
    }
}

void WindowListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eWindowResize) {
        const auto& resize_event = static_cast<const vne::events::WindowResizeEvent&>(event);
        VNE_LOG_INFO << "  Received: " << resize_event.toString();
    } else if (event.type() == vne::events::EventType::eWindowClose) {
        VNE_LOG_INFO << "  Received: WindowCloseEvent";
    }
}

}  // namespace vne::events::examples
