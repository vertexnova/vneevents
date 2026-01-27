/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Game loop integration demonstration implementation
 * ----------------------------------------------------------------------
 */

#include "game_loop_demo.h"

#include "listeners.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

#include <thread>

namespace  {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.game_loop_integration")
}

namespace vne::events::examples {

void GameLoopDemo::run() {
    VNE_LOG_INFO << "=== Game Loop Integration Demonstration ===";
    VNE_LOG_INFO << "";

    auto& manager = vne::events::EventManager::instance();
    auto game_listener = std::make_shared<GameEventListener>(&running_);

    manager.registerListener(vne::events::EventType::eWindowClose, game_listener);
    manager.registerListener(vne::events::EventType::eWindowResize, game_listener);
    manager.registerListener(vne::events::EventType::eKeyPressed, game_listener);

    VNE_LOG_INFO << "--- Game Loop Simulation ---";
    VNE_LOG_INFO << "  Registered event listeners";
    VNE_LOG_INFO << "  Starting game loop (5 frames)...";
    VNE_LOG_INFO << "";

    simulateGameLoop(5);

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demonstration Complete ===";
}

void GameLoopDemo::simulateGameLoop(int num_frames) {
    const float frame_time = 1.0f / 60.0f;  // 60 FPS

    for (int frame = 0; frame < num_frames && running_; ++frame) {
        frame_count_ = frame + 1;

        VNE_LOG_INFO << "--- Frame " << frame_count_ << " ---";

        // Step 1: Process window/input events (from platform callbacks)
        // In a real game, this would be glfwPollEvents() or similar
        processEvents();

        // Step 2: Update game state based on input polling
        update(frame_time);

        // Step 3: Render
        render();

        // Step 4: Reset per-frame input state
        vne::events::Input::nextFrame();

        VNE_LOG_INFO << "";

        // Simulate frame time
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void GameLoopDemo::processEvents() {
    auto& manager = vne::events::EventManager::instance();

    // Simulate some events being pushed (in real game, these come from platform callbacks)
    if (frame_count_ == 1) {
        // Simulate window resize
        manager.pushEvent(std::make_unique<vne::events::WindowResizeEvent>(1920, 1080));
    } else if (frame_count_ == 3) {
        // Simulate key press
        manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(vne::events::KeyCode::eSpace));
        // Update input state (simulating platform callback)
        vne::events::Input::updateKeyState(static_cast<int>(vne::events::KeyCode::eSpace), true);
    } else if (frame_count_ == 5) {
        // Simulate window close
        manager.pushEvent(std::make_unique<vne::events::WindowCloseEvent>());
    }

    // Process all pending events
    manager.processEvents();
}

void GameLoopDemo::update(float delta_time) {
    const float move_speed = 100.0f;  // pixels per second

    // Poll input state (continuous input)
    bool move_up = vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eW));
    bool move_down = vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eS));
    bool move_left = vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eA));
    bool move_right = vne::events::Input::isKeyPressed(static_cast<int>(vne::events::KeyCode::eD));

    // Handle just-pressed events (discrete actions)
    if (vne::events::Input::isKeyJustPressed(static_cast<int>(vne::events::KeyCode::eSpace))) {
        VNE_LOG_INFO << "    [Update] Space key just pressed - jump action!";
    }

    // Update player position based on input
    float delta_x = 0.0f;
    float delta_y = 0.0f;

    if (move_up) {
        delta_y -= move_speed * delta_time;
    }
    if (move_down) {
        delta_y += move_speed * delta_time;
    }
    if (move_left) {
        delta_x -= move_speed * delta_time;
    }
    if (move_right) {
        delta_x += move_speed * delta_time;
    }

    if (delta_x != 0.0f || delta_y != 0.0f) {
        player_x_ += delta_x;
        player_y_ += delta_y;
        VNE_LOG_INFO << "    [Update] Player moved to: (" << player_x_ << ", " << player_y_ << ")";
    } else {
        VNE_LOG_INFO << "    [Update] No movement input";
    }

    // Poll mouse state
    auto [mouse_x, mouse_y] = vne::events::Input::mousePosition();
    if (mouse_x != 0 || mouse_y != 0) {
        VNE_LOG_INFO << "    [Update] Mouse position: (" << mouse_x << ", " << mouse_y << ")";
    }

    if (vne::events::Input::isMouseButtonJustPressed(static_cast<int>(vne::events::MouseButton::eLeft))) {
        VNE_LOG_INFO << "    [Update] Left mouse button just clicked!";
    }
}

void GameLoopDemo::render() {
    // In a real game, this would render the scene
    VNE_LOG_INFO << "    [Render] Rendering frame (player at: " << player_x_ << ", " << player_y_ << ")";
}

}  // namespace vne::events::examples
