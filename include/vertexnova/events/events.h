#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Main include header for VertexNova Events library.
 * ----------------------------------------------------------------------
 */

/**
 * @file events.h
 * @brief VertexNova Events - A lightweight, thread-safe event system.
 *
 * This library provides:
 * - Event base class and concrete event types (keyboard, mouse, window, touch)
 * - Thread-safe event queue for deferred event processing
 * - Event dispatcher with listener registration
 * - Event bus combining queue and dispatcher
 * - Singleton event manager for global event handling
 * - Input state polling for keyboard, mouse, and gamepad
 *
 * @code
 * #include <vertexnova/events/events.h>
 *
 * // Register a listener
 * class MyListener : public vne::events::EventListener {
 * public:
 *     void onEvent(const vne::events::Event& event) override {
 *         std::cout << event.toString() << std::endl;
 *     }
 * };
 *
 * // Create and push events
 * auto& manager = vne::events::EventManager::instance();
 * manager.registerListener(vne::events::EventType::eKeyPressed,
 *                          std::make_shared<MyListener>());
 * manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(
 *                          vne::events::KeyCode::eA));
 * manager.processEvents();
 * @endcode
 */

// Types and constants
#include "types.h"

// Core event system
#include "event.h"
#include "event_listener.h"
#include "event_queue.h"
#include "event_dispatcher.h"
#include "event_bus.h"
#include "event_manager.h"

// Concrete event types
#include "key_event.h"
#include "mouse_event.h"
#include "window_event.h"
#include "touch_event.h"

// Input polling
#include "input/input.h"
