/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener implementations for Multiple Listeners example
 * ----------------------------------------------------------------------
 */

#include "listeners.h"

#include <vertexnova/logging/logging.h>

namespace {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.event_listeners")
}

namespace vne::events::examples {

void AudioListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
        VNE_LOG_INFO << "  [Audio] Playing key press sound for key: " << static_cast<int>(key_event.keyCode());
    }
}

void UIListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
        VNE_LOG_INFO << "  [UI] Updating UI for key: " << static_cast<int>(key_event.keyCode());
    }
}

void GameplayListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
        VNE_LOG_INFO << "  [Gameplay] Handling key press: " << static_cast<int>(key_event.keyCode());
    }
}

void InputListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed || event.type() == vne::events::EventType::eKeyReleased
        || event.type() == vne::events::EventType::eMouseButtonPressed) {
        VNE_LOG_INFO << "  [Input] Handling input event: " << event.name();
    }
}

void WindowListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eWindowResize || event.type() == vne::events::EventType::eWindowClose) {
        VNE_LOG_INFO << "  [Window] Handling window event: " << event.name();
    }
}

void RenderListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eWindowResize) {
        const auto& resize_event = static_cast<const vne::events::WindowResizeEvent&>(event);
        VNE_LOG_INFO << "  [Render] Resizing render target to: " << resize_event.width() << "x"
                     << resize_event.height();
    }
}

TemporaryListener::TemporaryListener(const std::string& name)
    : name_(name) {}

void TemporaryListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eKeyPressed) {
        const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
        VNE_LOG_INFO << "  [" << name_ << "] Received key: " << static_cast<int>(key_event.keyCode());
    }
}

}  // namespace vne::events::examples
