# 04 - Multithreaded Events

This example demonstrates thread-safe event processing with concurrent event pushing and listener registration.

## What This Example Shows

1. **Concurrent Event Pushing**: Multiple threads pushing events simultaneously
2. **Thread-Safe Event Queue**: Events are safely queued from multiple threads
3. **Concurrent Listener Registration**: Listeners can be registered from different threads
4. **Thread-Safe Processing**: Event processing is safe even with concurrent pushing
5. **No Race Conditions**: All events are processed correctly without corruption

## Building

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build .
```

## Running

```bash
./bin/examples/example_04_multithreaded_events
```

## Expected Output

```
[INFO] [vneevents.examples] VneEvents Example: Multithreaded Events
[INFO] [vneevents.examples] =======================================
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Multithreaded Events Demonstration ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Concurrent Event Pushing ---
[INFO] [vneevents.examples]   Starting 4 threads
[INFO] [vneevents.examples]   Each thread will push 10 events
[INFO] [vneevents.examples]   Total events expected: 40
[INFO] [vneevents.examples]   All threads completed pushing events
[INFO] [vneevents.examples]   Processing events...
[INFO] [vneevents.examples]   Events received: 40
[INFO] [vneevents.examples]   Expected: 40
[INFO] [vneevents.examples]   Thread-safe: ✓ Yes
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Concurrent Listener Registration ---
[INFO] [vneevents.examples]   Registering 3 listeners from different threads
[INFO] [vneevents.examples]   All listeners registered
[INFO] [vneevents.examples]   Pushing test event...
[INFO] [vneevents.examples]     [Thread 0] Received key event
[INFO] [vneevents.examples]     [Thread 1] Received key event
[INFO] [vneevents.examples]     [Thread 2] Received key event
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Thread-Safe Event Processing ---
[INFO] [vneevents.examples]   Testing thread-safe event queue
[INFO] [vneevents.examples]   5 threads pushing events concurrently
[INFO] [vneevents.examples]   Processing events after all threads complete
[INFO] [vneevents.examples]   All threads completed pushing events
[INFO] [vneevents.examples]   Processing all events...
[INFO] [vneevents.examples]   Events received: 100
[INFO] [vneevents.examples]   Expected: 100
[INFO] [vneevents.examples]   Thread-safe: ✓ Yes
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Demonstration Complete ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Done ===
```

## Key Concepts

### Thread Safety Guarantees

The vneevents library provides thread-safe operations:

- **EventQueue**: Thread-safe push and pop operations using read-write locks
- **EventDispatcher**: Thread-safe listener registration and dispatch
- **EventManager**: All public methods are thread-safe
- **Input State**: Thread-safe queries and updates

### Concurrent Event Pushing

Multiple threads can safely push events simultaneously:

```cpp
// Thread 1
manager.pushEvent(std::make_unique<KeyPressedEvent>(KeyCode::eW));

// Thread 2 (concurrent)
manager.pushEvent(std::make_unique<MouseMovedEvent>(100, 200));

// Both events are safely queued
```

### Concurrent Listener Registration

Listeners can be registered from any thread:

```cpp
// Thread 1
manager.registerListener(EventType::eKeyPressed, listener1);

// Thread 2 (concurrent)
manager.registerListener(EventType::eKeyPressed, listener2);

// Both listeners are safely registered
```

### Thread-Safe Processing

Events can be processed while other threads are pushing:

```cpp
// Main thread
while (running) {
    manager.processEvents();  // Safe even if other threads are pushing
}

// Worker thread (concurrent)
manager.pushEvent(std::make_unique<KeyPressedEvent>(KeyCode::eSpace));
```

## Implementation Details

### ReadWriteMutex

The library uses `ReadWriteMutex` internally:
- **Read locks**: For querying state (multiple readers allowed)
- **Write locks**: For modifying state (exclusive access)

### Atomic Operations

Event counting uses `std::atomic<int>` to ensure thread-safe increments.

### No Data Races

All event operations are protected by appropriate locks, ensuring:
- No event loss
- No event duplication
- No listener corruption
- Correct event ordering

## Code Structure

- `main.cpp`: Minimal setup, uses `LoggingGuard_C` and calls `ThreadedDemo::run()`
- `threaded_demo.h` / `threaded_demo.cpp`: Helper class encapsulating the multithreaded demonstration
- `common/logging_guard.h`: Shared logging configuration for all examples

## Use Cases

- **Game Engine**: Multiple systems (audio, physics, render) pushing events from different threads
- **Multi-threaded Applications**: Worker threads generating events, main thread processing
- **Plugin Systems**: Plugins registering listeners from different threads
- **Async Operations**: Background tasks pushing completion events
