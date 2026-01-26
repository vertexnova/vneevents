# 05 - Game Loop Integration

This example demonstrates a complete game loop integration combining event-driven architecture with input polling.

## What This Example Shows

1. **Game Loop Structure**: Complete game loop with event processing, update, and render phases
2. **Event Processing**: Processing events at the start of each frame
3. **Input Polling**: Polling input state during the update phase
4. **Combined Approach**: Using both events and polling together
5. **Frame-Based State**: Per-frame input state management with `nextFrame()`

## Building

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build .
```

## Running

```bash
./bin/examples/example_05_game_loop_integration
```

## Expected Output

```
[INFO] [vneevents.examples] VneEvents Example: Game Loop Integration
[INFO] [vneevents.examples] ========================================
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Game Loop Integration Demonstration ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Game Loop Simulation ---
[INFO] [vneevents.examples]   Registered event listeners
[INFO] [vneevents.examples]   Starting game loop (5 frames)...
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Frame 1 ---
[INFO] [vneevents.examples]   [Event] Window resized to: 1920x1080
[INFO] [vneevents.examples]     [Update] No movement input
[INFO] [vneevents.examples]     [Render] Rendering frame (player at: 0, 0)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Frame 2 ---
[INFO] [vneevents.examples]     [Update] No movement input
[INFO] [vneevents.examples]     [Render] Rendering frame (player at: 0, 0)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Frame 3 ---
[INFO] [vneevents.examples]   [Event] Key pressed: 32
[INFO] [vneevents.examples]     [Update] Space key just pressed - jump action!
[INFO] [vneevents.examples]     [Update] No movement input
[INFO] [vneevents.examples]     [Render] Rendering frame (player at: 0, 0)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Frame 4 ---
[INFO] [vneevents.examples]     [Update] No movement input
[INFO] [vneevents.examples]     [Render] Rendering frame (player at: 0, 0)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Frame 5 ---
[INFO] [vneevents.examples]   [Event] Window close requested
[INFO] [vneevents.examples]     [Update] No movement input
[INFO] [vneevents.examples]     [Render] Rendering frame (player at: 0, 0)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Demonstration Complete ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Done ===
```

## Key Concepts

### Game Loop Structure

A typical game loop follows this pattern:

```cpp
while (running) {
    // 1. Process events (from platform callbacks)
    processEvents();
    
    // 2. Update game state (poll input, update entities)
    update(delta_time);
    
    // 3. Render the frame
    render();
    
    // 4. Reset per-frame input state
    Input::nextFrame();
}
```

### Event Processing

- Events are pushed by platform callbacks (GLFW, SDL, etc.)
- Process all events at the start of each frame
- Events handle discrete actions (window close, key press, etc.)

### Input Polling

- Poll input state during the update phase
- Use `isKeyPressed()` for continuous input (movement)
- Use `isKeyJustPressed()` for single-press actions (jump, shoot)
- Always call `nextFrame()` at the end of the frame

### Combined Approach

- **Events**: Best for discrete actions (window close, key press events)
- **Polling**: Best for continuous input (movement, camera control)
- **Both**: Use events to update input state, poll for game logic

## Code Structure

- `main.cpp`: Minimal setup, uses `LoggingGuard_C` and calls `GameLoopDemo::run()`
- `game_loop_demo.h` / `game_loop_demo.cpp`: Helper class encapsulating the game loop logic
- `common/logging_guard.h`: Shared logging configuration for all examples

## Real-World Usage

In a real game engine:

1. **Platform Layer**: Window callbacks push events and update input state
2. **Event System**: Processes events for UI, window management, etc.
3. **Game Logic**: Polls input state for player movement, camera control
4. **Render System**: Renders the scene based on current game state

This example simulates this architecture without requiring a windowing system.
