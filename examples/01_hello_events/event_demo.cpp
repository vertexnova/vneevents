/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event demonstration implementation
 * ----------------------------------------------------------------------
 */

#include "event_demo.h"

#include "common/logging_guard.h"

#include <vertexnova/logging/logging.h>

namespace vne::events::examples {

// Forward declarations for listener classes
class KeyboardListener;
class MouseListener;
class WindowListener;

void EventDemo::run() {
    VNE_LOG_INFO << "=== Event System Demonstration ===";
    VNE_LOG_INFO << "";

    demonstrateKeyboardEvents();
    demonstrateMouseEvents();
    demonstrateWindowEvents();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demonstration Complete ===";
}

void EventDemo::demonstrateKeyboardEvents() {
    VNE_LOG_INFO << "--- Keyboard Events ---";

    // Create a listener for keyboard events
    class KeyboardListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eKeyPressed) {
                const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
                VNE_LOG_INFO << "  Received: " << key_event.toString();
            } else if (event.type() == vne::events::EventType::eKeyReleased) {
                const auto& key_event = static_cast<const vne::events::KeyReleasedEvent&>(event);
                VNE_LOG_INFO << "  Received: " << key_event.toString();
            }
        }
    };

    auto& manager = vne::events::EventManager::instance();
    auto keyboard_listener = std::make_shared<KeyboardListener>();

    // Register listener for key events
    manager.registerListener(vne::events::EventType::eKeyPressed, keyboard_listener);
    manager.registerListener(vne::events::EventType::eKeyReleased, keyboard_listener);

    VNE_LOG_INFO << "  Registered keyboard listeners";
    VNE_LOG_INFO << "  Pushing key events...";

    // Push some keyboard events
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eW));
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eA));
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eS));
    manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eD));
    manager.pushEvent(std::make_unique<vne::events::KeyReleasedEvent>(vne::events::KeyCode::eW));

    VNE_LOG_INFO << "  Processing events...";
    manager.processEvents();

    VNE_LOG_INFO << "";
}

void EventDemo::demonstrateMouseEvents() {
    VNE_LOG_INFO << "--- Mouse Events ---";

    // Create a listener for mouse events
    class MouseListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eMouseMoved) {
                const auto& mouse_event = static_cast<const vne::events::MouseMovedEvent&>(event);
                VNE_LOG_INFO << "  Received: " << mouse_event.toString();
            } else if (event.type() == vne::events::EventType::eMouseButtonPressed) {
                const auto& button_event =
                    static_cast<const vne::events::MouseButtonPressedEvent&>(event);
                VNE_LOG_INFO << "  Received: " << button_event.toString();
            } else if (event.type() == vne::events::EventType::eMouseScrolled) {
                const auto& scroll_event = static_cast<const vne::events::MouseScrolledEvent&>(event);
                VNE_LOG_INFO << "  Received: " << scroll_event.toString();
            }
        }
    };

    auto& manager = vne::events::EventManager::instance();
    auto mouse_listener = std::make_shared<MouseListener>();

    // Register listener for mouse events
    manager.registerListener(vne::events::EventType::eMouseMoved, mouse_listener);
    manager.registerListener(vne::events::EventType::eMouseButtonPressed, mouse_listener);
    manager.registerListener(vne::events::EventType::eMouseScrolled, mouse_listener);

    VNE_LOG_INFO << "  Registered mouse listeners";
    VNE_LOG_INFO << "  Pushing mouse events...";

    // Push some mouse events
    manager.pushEvent(std::make_unique<vne::events::MouseMovedEvent>(100.0, 200.0));
    manager.pushEvent(std::make_unique<vne::events::MouseMovedEvent>(150.0, 250.0));
    manager.pushEvent(
        std::make_unique<vne::events::MouseButtonPressedEvent>(vne::events::MouseButton::eLeft));
    manager.pushEvent(std::make_unique<vne::events::MouseScrolledEvent>(0.0, 1.0));

    VNE_LOG_INFO << "  Processing events...";
    manager.processEvents();

    VNE_LOG_INFO << "";
}

void EventDemo::demonstrateWindowEvents() {
    VNE_LOG_INFO << "--- Window Events ---";

    // Create a listener for window events
    class WindowListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eWindowResize) {
                const auto& resize_event = static_cast<const vne::events::WindowResizeEvent&>(event);
                VNE_LOG_INFO << "  Received: " << resize_event.toString();
            } else if (event.type() == vne::events::EventType::eWindowClose) {
                VNE_LOG_INFO << "  Received: WindowCloseEvent";
            }
        }
    };

    auto& manager = vne::events::EventManager::instance();
    auto window_listener = std::make_shared<WindowListener>();

    // Register listener for window events
    manager.registerListener(vne::events::EventType::eWindowResize, window_listener);
    manager.registerListener(vne::events::EventType::eWindowClose, window_listener);

    VNE_LOG_INFO << "  Registered window listeners";
    VNE_LOG_INFO << "  Pushing window events...";

    // Push some window events
    manager.pushEvent(std::make_unique<vne::events::WindowResizeEvent>(1920, 1080));
    manager.pushEvent(std::make_unique<vne::events::WindowResizeEvent>(1280, 720));

    VNE_LOG_INFO << "  Processing events...";
    manager.processEvents();

    VNE_LOG_INFO << "";
}

}  // namespace vne::events::examples
