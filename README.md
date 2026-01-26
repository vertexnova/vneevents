# VneEvents

A lightweight, thread-safe event system for the VertexNova Engine.

[![CI](https://github.com/vertexnova/vneevents/actions/workflows/ci.yml/badge.svg)](https://github.com/vertexnova/vneevents/actions/workflows/ci.yml)

## Features

- **Type-safe events**: Strongly typed event classes for keyboard, mouse, window, and touch inputs
- **Thread-safe**: All event queue and dispatcher operations are protected by read-write locks
- **Flexible architecture**: Use the EventBus for queued processing or dispatch events immediately
- **Input polling**: Static `Input` class for polling keyboard, mouse, and gamepad state
- **Zero dependencies**: Header-only core components (except for input state tracking)
- **Modern C++17**: Uses `std::shared_mutex`, `[[nodiscard]]`, and structured bindings

## Quick Start

### Include the Library

```cpp
#include <vertexnova/events/events.h>

using namespace vne::events;
```

### Create an Event Listener

```cpp
class MyListener : public EventListener {
public:
    void onEvent(const Event& event) override {
        if (event.type() == EventType::eKeyPressed) {
            auto& key_event = static_cast<const KeyPressedEvent&>(event);
            std::cout << "Key pressed: " << static_cast<int>(key_event.keyCode()) << std::endl;
        }
    }
};
```

### Register and Process Events

```cpp
auto& manager = EventManager::instance();

// Register listener
auto listener = std::make_shared<MyListener>();
manager.registerListener(EventType::eKeyPressed, listener);

// Push events
manager.pushEvent(std::make_unique<KeyPressedEvent>(KeyCode::eA));
manager.pushEvent(std::make_unique<MouseMovedEvent>(100.0, 200.0));

// Process all pending events
manager.processEvents();
```

### Poll Input State

```cpp
// In your game loop
if (Input::isKeyPressed(static_cast<int>(KeyCode::eW))) {
    // Move forward
}

if (Input::isMouseButtonJustPressed(0)) {
    auto [x, y] = Input::mousePosition();
    // Handle click at (x, y)
}

// At end of frame
Input::nextFrame();
```

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                       EventManager                           │
│  ┌─────────────────┐        ┌─────────────────┐             │
│  │   EventQueue    │───────>│   EventBus      │             │
│  │  (thread-safe)  │        │                 │             │
│  └─────────────────┘        │  ┌───────────┐  │             │
│                             │  │Dispatcher │  │             │
│                             │  └─────┬─────┘  │             │
│                             └────────┼────────┘             │
└──────────────────────────────────────┼──────────────────────┘
                                       │
                    ┌──────────────────┼──────────────────┐
                    │                  │                  │
              ┌─────▼─────┐     ┌──────▼──────┐    ┌──────▼──────┐
              │ Listener1 │     │  Listener2  │    │  Listener3  │
              └───────────┘     └─────────────┘    └─────────────┘
```

## Event Types

| Event Class | EventType | Category |
|-------------|-----------|----------|
| `KeyPressedEvent` | `eKeyPressed` | Keyboard, Input |
| `KeyReleasedEvent` | `eKeyReleased` | Keyboard, Input |
| `KeyRepeatEvent` | `eKeyRepeat` | Keyboard, Input |
| `KeyTypedEvent` | `eKeyTyped` | Keyboard, Input |
| `MouseButtonPressedEvent` | `eMouseButtonPressed` | MouseButton, Input |
| `MouseButtonReleasedEvent` | `eMouseButtonReleased` | MouseButton, Input |
| `MouseMovedEvent` | `eMouseMoved` | Mouse, Input |
| `MouseScrolledEvent` | `eMouseScrolled` | Mouse, Input |
| `WindowCloseEvent` | `eWindowClose` | Window |
| `WindowResizeEvent` | `eWindowResize` | Window |
| `TouchPressEvent` | `eTouchPress` | TouchScreen, Input |
| `TouchReleaseEvent` | `eTouchRelease` | TouchScreen, Input |
| `TouchMoveEvent` | `eTouchMove` | TouchScreen, Input |

## Building

### Requirements

- CMake 3.16+
- C++17 compiler (GCC 8+, Clang 7+, MSVC 2019+)

### Build Steps

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
cmake --build .
ctest
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | ON | Build the test suite |
| `BUILD_EXAMPLES` | OFF | Build example programs |
| `ENABLE_COVERAGE` | OFF | Enable code coverage |

## Integration

### As a Submodule

```bash
git submodule add https://github.com/vertexnova/vneevents.git libs/vneevents
```

```cmake
add_subdirectory(libs/vneevents)
target_link_libraries(your_target PRIVATE vne::events)
```

### Using find_package

```cmake
find_package(VneEvents REQUIRED)
target_link_libraries(your_target PRIVATE vne::events)
```

## Platform Support

| Platform | Status |
|----------|--------|
| Windows | ✅ |
| Linux | ✅ |
| macOS | ✅ |
| iOS | ✅ |
| Android | ✅ |
| Web (Emscripten) | ✅ |

## License

Licensed under the Apache License, Version 2.0.

## Author

Ajeet Singh Yadav - [yadav.ajeetsingh2020@gmail.com](mailto:yadav.ajeetsingh2020@gmail.com)
