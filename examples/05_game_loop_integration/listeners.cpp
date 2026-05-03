/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener implementations for Game Loop Integration example
 * ----------------------------------------------------------------------
 */

#include "listeners.h"

#include <vertexnova/logging/logging.h>

namespace {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.game_loop_integration")
}

namespace vne::events::examples {

GameEventListener::GameEventListener(bool* running)
    : running_(running) {}

void GameEventListener::onEvent(const vne::events::Event& event) {
    if (event.type() == vne::events::EventType::eWindowClose) {
        VNE_LOG_INFO << "  [Event] Window close requested";
        *running_ = false;
    } else if (event.type() == vne::events::EventType::eWindowResize) {
        const auto& resize_event = static_cast<const vne::events::WindowResizeEvent&>(event);
        VNE_LOG_INFO << "  [Event] Window resized to: " << resize_event.width() << "x" << resize_event.height();
    } else if (event.type() == vne::events::EventType::eKeyPressed) {
        const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
        VNE_LOG_INFO << "  [Event] Key pressed: " << static_cast<int>(key_event.keyCode());
    }
}

}  // namespace vne::events::examples
