# 03 - Event Listeners

This example demonstrates using multiple event listeners for different event types and subsystems.

## What This Example Shows

1. **Multiple Listeners**: Registering multiple listeners for the same event type
2. **Subsystem Listeners**: Separate listeners for different subsystems (Input, Window, Render)
3. **Event Filtering**: Listeners can filter events by type
4. **Listener Lifecycle**: Registering and unregistering listeners dynamically

## Building

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build .
```

## Running

```bash
./bin/examples/example_03_event_listeners
```

## Expected Output

```
[INFO] [vneevents.examples] VneEvents Example: Event Listeners
[INFO] [vneevents.examples] ===================================
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Multiple Event Listeners Demonstration ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Multiple Listeners for Same Event Type ---
[INFO] [vneevents.examples]   Registered 3 listeners for KeyPressed events
[INFO] [vneevents.examples]   Pushing key event...
[INFO] [vneevents.examples]   [Audio] Playing key press sound for key: 32
[INFO] [vneevents.examples]   [UI] Updating UI for key: 32
[INFO] [vneevents.examples]   [Gameplay] Handling key press: 32
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Subsystem-Specific Listeners ---
[INFO] [vneevents.examples]   Registered subsystem listeners:
[INFO] [vneevents.examples]     Input: KeyPressed, KeyReleased, MouseButtonPressed
[INFO] [vneevents.examples]     Window: WindowResize, WindowClose
[INFO] [vneevents.examples]     Render: WindowResize
[INFO] [vneevents.examples]   Pushing events...
[INFO] [vneevents.examples]   [Input] Handling input event: KeyPressed
[INFO] [vneevents.examples]   [Input] Handling input event: KeyReleased
[INFO] [vneevents.examples]   [Input] Handling input event: MouseButtonPressed
[INFO] [vneevents.examples]   [Window] Handling window event: WindowResize
[INFO] [vneevents.examples]   [Render] Resizing render target to: 1920x1080
[INFO] [vneevents.examples]   [Window] Handling window event: WindowClose
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Listener Lifecycle Management ---
[INFO] [vneevents.examples]   Registered temporary listener
[INFO] [vneevents.examples]   Pushing event (listener active)...
[INFO] [vneevents.examples]   [Temporary] Received key: 256
[INFO] [vneevents.examples]   Unregistered temporary listener
[INFO] [vneevents.examples]   Pushing event (listener inactive)...
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Demonstration Complete ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Done ===
```

## Key Concepts

- **Multiple Listeners**: You can register multiple listeners for the same event type - all will receive the event
- **Subsystem Pattern**: Different subsystems can have their own listeners for relevant event types
- **Event Filtering**: Listeners can check event types and handle only relevant events
- **Listener Registration**: Use `EventManager::registerListener()` to add listeners
- **Listener Unregistration**: Use `EventManager::unregisterListener()` to remove listeners
- **Listener Lifetime**: Listeners are managed via `std::shared_ptr` - they remain active as long as they're registered

## Code Structure

- `main.cpp`: Minimal setup, uses `LoggingGuard_C` and calls `ListenerDemo::run()`
- `listener_demo.h` / `listener_demo.cpp`: Helper class encapsulating the demonstration logic
- `common/logging_guard.h`: Shared logging configuration for all examples

## Use Cases

- **Game Engine Architecture**: Different subsystems (Audio, UI, Gameplay, Render) can listen to relevant events
- **Plugin System**: Plugins can register listeners for events they care about
- **Debug/Logging**: Separate listeners for logging or debugging specific event types
- **Input Mapping**: Multiple listeners can handle the same input event differently
