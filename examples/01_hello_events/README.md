# 01 - Hello Events

This example demonstrates the fundamental usage of the VneEvents library.

## What This Example Shows

1. **Event Listeners**: Creating custom event listener classes
2. **Event Registration**: Registering listeners with EventManager
3. **Event Pushing**: Creating and pushing events to the queue
4. **Event Processing**: Processing queued events and dispatching to listeners
5. **Event Types**: Handling keyboard, mouse, and window events

## Building

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build .
```

## Running

```bash
./bin/examples/example_01_hello_events
```

## Expected Output

```
[INFO] [vneevents.examples] VneEvents Example: Hello Events
[INFO] [vneevents.examples] ===============================
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Event System Demonstration ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Keyboard Events ---
[INFO] [vneevents.examples]   Registered keyboard listeners
[INFO] [vneevents.examples]   Pushing key events...
[INFO] [vneevents.examples]   Processing events...
[INFO] [vneevents.examples]   Received: KeyPressedEvent: 87
[INFO] [vneevents.examples]   Received: KeyPressedEvent: 65
[INFO] [vneevents.examples]   Received: KeyPressedEvent: 83
[INFO] [vneevents.examples]   Received: KeyPressedEvent: 68
[INFO] [vneevents.examples]   Received: KeyReleasedEvent: 87
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Mouse Events ---
[INFO] [vneevents.examples]   Registered mouse listeners
[INFO] [vneevents.examples]   Pushing mouse events...
[INFO] [vneevents.examples]   Processing events...
[INFO] [vneevents.examples]   Received: MouseMovedEvent: (100, 200)
[INFO] [vneevents.examples]   Received: MouseMovedEvent: (150, 250)
[INFO] [vneevents.examples]   Received: MouseButtonPressedEvent: 0
[INFO] [vneevents.examples]   Received: MouseButtonDoubleClickedEvent: 0
[INFO] [vneevents.examples]   Received: MouseScrolledEvent: (0, 1)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Window Events ---
[INFO] [vneevents.examples]   Registered window listeners
[INFO] [vneevents.examples]   Pushing window events...
[INFO] [vneevents.examples]   Processing events...
[INFO] [vneevents.examples]   Received: WindowResizeEvent: (1920, 1080)
[INFO] [vneevents.examples]   Received: WindowResizeEvent: (1280, 720)
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Demonstration Complete ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Done ===
```

## Key Concepts

- **EventListener**: Interface class that must be implemented to receive events
- **EventManager**: Singleton that manages event listeners and processing
- **Event Queue**: Events are pushed to a queue and processed later
- **Event Types**: Different event types (keyboard, mouse, window) for different input sources
- **Helper Classes**: Example logic is separated into `EventDemo` helper class instead of being in `main()`

## Code Structure

- `main.cpp`: Minimal setup, uses `LoggingGuard_C` and calls `EventDemo::run()`
- `event_demo.h` / `event_demo.cpp`: Helper class encapsulating the demonstration logic
- `common/logging_guard.h`: Shared logging configuration for all examples
