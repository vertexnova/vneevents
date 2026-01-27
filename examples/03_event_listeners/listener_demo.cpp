/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Multiple event listeners demonstration implementation
 * ----------------------------------------------------------------------
 */

#include "listener_demo.h"

#include "common/logging_guard.h"
#include "listeners.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

#include <memory>

namespace vne::events::examples {

void ListenerDemo::run() {
    VNE_LOG_INFO << "=== Multiple Event Listeners Demonstration ===";
    VNE_LOG_INFO << "";

    demonstrateMultipleListeners();
    demonstrateSubsystemListeners();
    demonstrateListenerLifecycle();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demonstration Complete ===";
}

void ListenerDemo::demonstrateMultipleListeners() {
    VNE_LOG_INFO << "--- Multiple Listeners for Same Event Type ---";

    auto& manager = vne::events::EventManager::instance();
    auto audio_listener = std::make_shared<AudioListener>();
    auto ui_listener = std::make_shared<UIListener>();
    auto gameplay_listener = std::make_shared<GameplayListener>();

    // Register all three listeners for the same event type
    manager.registerListener(vne::events::EventType::eKeyPressed, audio_listener);
    manager.registerListener(vne::events::EventType::eKeyPressed, ui_listener);
    manager.registerListener(vne::events::EventType::eKeyPressed, gameplay_listener);

    VNE_LOG_INFO << "  Registered 3 listeners for KeyPressed events";
    VNE_LOG_INFO << "  Pushing key event...";

    // Push a single key event - all three listeners should receive it
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eSpace));
    manager.processEvents();

    VNE_LOG_INFO << "";
}

void ListenerDemo::demonstrateSubsystemListeners() {
    VNE_LOG_INFO << "--- Subsystem-Specific Listeners ---";

    auto& manager = vne::events::EventManager::instance();
    auto input_listener = std::make_shared<InputListener>();
    auto window_listener = std::make_shared<WindowListener>();
    auto render_listener = std::make_shared<RenderListener>();

    // Register listeners for their respective event types
    manager.registerListener(vne::events::EventType::eKeyPressed, input_listener);
    manager.registerListener(vne::events::EventType::eKeyReleased, input_listener);
    manager.registerListener(vne::events::EventType::eMouseButtonPressed, input_listener);
    manager.registerListener(vne::events::EventType::eWindowResize, window_listener);
    manager.registerListener(vne::events::EventType::eWindowResize, render_listener);
    manager.registerListener(vne::events::EventType::eWindowClose, window_listener);

    VNE_LOG_INFO << "  Registered subsystem listeners:";
    VNE_LOG_INFO << "    Input: KeyPressed, KeyReleased, MouseButtonPressed";
    VNE_LOG_INFO << "    Window: WindowResize, WindowClose";
    VNE_LOG_INFO << "    Render: WindowResize";
    VNE_LOG_INFO << "  Pushing events...";

    // Push various events
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eW));
    manager.pushEvent(std::make_unique<vne::events::KeyReleasedEvent>(vne::events::KeyCode::eW));
    manager.pushEvent(std::make_unique<vne::events::MouseButtonPressedEvent>(vne::events::MouseButton::eLeft));
    manager.pushEvent(std::make_unique<vne::events::WindowResizeEvent>(1920, 1080));
    manager.pushEvent(std::make_unique<vne::events::WindowCloseEvent>());

    manager.processEvents();

    VNE_LOG_INFO << "";
}

void ListenerDemo::demonstrateListenerLifecycle() {
    VNE_LOG_INFO << "--- Listener Lifecycle Management ---";

    auto& manager = vne::events::EventManager::instance();

    // Register a temporary listener
    auto temp_listener = std::make_shared<TemporaryListener>("Temporary");
    manager.registerListener(vne::events::EventType::eKeyPressed, temp_listener);

    VNE_LOG_INFO << "  Registered temporary listener";
    VNE_LOG_INFO << "  Pushing event (listener active)...";
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eEscape));
    manager.processEvents();

    // Unregister the listener
    manager.unregisterListener(vne::events::EventType::eKeyPressed, temp_listener.get());
    VNE_LOG_INFO << "  Unregistered temporary listener";
    VNE_LOG_INFO << "  Pushing event (listener inactive)...";
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eEscape));
    manager.processEvents();

    VNE_LOG_INFO << "";
}

}  // namespace vne::events::examples
