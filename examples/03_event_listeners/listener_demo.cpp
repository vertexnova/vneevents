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

    // Create multiple listeners for key pressed events
    class AudioListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eKeyPressed) {
                const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
                VNE_LOG_INFO << "  [Audio] Playing key press sound for key: " << static_cast<int>(key_event.keyCode());
            }
        }
    };

    class UIListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eKeyPressed) {
                const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
                VNE_LOG_INFO << "  [UI] Updating UI for key: " << static_cast<int>(key_event.keyCode());
            }
        }
    };

    class GameplayListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eKeyPressed) {
                const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
                VNE_LOG_INFO << "  [Gameplay] Handling key press: " << static_cast<int>(key_event.keyCode());
            }
        }
    };

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

    // Input subsystem listener
    class InputListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eKeyPressed ||
                event.type() == vne::events::EventType::eKeyReleased ||
                event.type() == vne::events::EventType::eMouseButtonPressed) {
                VNE_LOG_INFO << "  [Input] Handling input event: " << event.name();
            }
        }
    };

    // Window subsystem listener
    class WindowListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eWindowResize ||
                event.type() == vne::events::EventType::eWindowClose) {
                VNE_LOG_INFO << "  [Window] Handling window event: " << event.name();
            }
        }
    };

    // Render subsystem listener
    class RenderListener : public vne::events::EventListener {
       public:
        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eWindowResize) {
                const auto& resize_event = static_cast<const vne::events::WindowResizeEvent&>(event);
                VNE_LOG_INFO << "  [Render] Resizing render target to: " << resize_event.width() << "x" << resize_event.height();
            }
        }
    };

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

    class TemporaryListener : public vne::events::EventListener {
       public:
        explicit TemporaryListener(const std::string& name)
            : name_(name) {}

        void onEvent(const vne::events::Event& event) override {
            if (event.type() == vne::events::EventType::eKeyPressed) {
                const auto& key_event = static_cast<const vne::events::KeyPressedEvent&>(event);
                VNE_LOG_INFO << "  [" << name_ << "] Received key: " << static_cast<int>(key_event.keyCode());
            }
        }

       private:
        std::string name_;
    };

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
