#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Game loop integration demonstration helper class
 * ----------------------------------------------------------------------
 */

#include <chrono>
#include <memory>

namespace vne::events::examples {

/**
 * @class GameLoopDemo
 * @brief Helper class that demonstrates a complete game loop integration.
 *
 * This class demonstrates:
 * - Event-driven architecture
 * - Input polling in update loop
 * - Event processing per frame
 * - Combined event + polling approach
 * - Frame-based state management
 */
class GameLoopDemo {
   public:
    GameLoopDemo() = default;
    ~GameLoopDemo() = default;

    // Non-copyable, movable
    GameLoopDemo(const GameLoopDemo&) = delete;
    GameLoopDemo& operator=(const GameLoopDemo&) = delete;
    GameLoopDemo(GameLoopDemo&&) noexcept = default;
    GameLoopDemo& operator=(GameLoopDemo&&) noexcept = default;

    /**
     * @brief Runs the game loop demonstration.
     */
    void run();

   private:
    /**
     * @brief Simulates a game loop with event processing and input polling.
     * @param num_frames Number of frames to simulate
     */
    void simulateGameLoop(int num_frames);

    /**
     * @brief Processes events for the current frame.
     */
    void processEvents();

    /**
     * @brief Updates game state based on input polling.
     * @param delta_time Time since last frame in seconds
     */
    void update(float delta_time);

    /**
     * @brief Renders the current frame (simulated).
     */
    void render();

    // Game state
    float player_x_ = 0.0f;
    float player_y_ = 0.0f;
    bool running_ = true;
    int frame_count_ = 0;
};

}  // namespace vne::events::examples
