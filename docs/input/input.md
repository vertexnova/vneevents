# Input System

## Overview

The Input System provides a unified interface for handling keyboard, mouse, and gamepad input across all supported platforms. It offers both polling and event-driven input handling with thread-safe state management and per-frame input tracking.

## Architecture

The Input System follows a layered architecture with clear separation between interface and implementation:

```
Input System
├── Input              # Static interface for input queries
├── InputManager       # Input device management and coordination
├── InputState         # Thread-safe input state tracking
└── Platform Integration # Platform-specific input handling
    ├── GLFW           # Desktop input integration
    ├── Mobile         # Touch input handling
    └── Gamepad        # Controller input support
```

### Key Design Principles

- **Unified API**: Single interface for all input types
- **Thread Safety**: Safe concurrent access to input state
- **Performance**: Efficient input polling and state tracking
- **Platform Abstraction**: Cross-platform input handling
- **Real-time State**: Per-frame input state updates

## Core Components

### Input - Static Interface

The primary interface for querying input state throughout the application.

**Key Features:**

- **Static Methods**: No instance required, easy to use anywhere
- **Per-frame Tracking**: Distinguishes between pressed and just-pressed states
- **Thread Safety**: Safe to call from any thread
- **Platform Independent**: Works across all supported platforms

### InputManager - Input Management

Manages input devices and coordinates input state updates. Provides singleton access to input state.

### InputState - State Management

Thread-safe input state tracking with per-frame state management using read-write locks for efficient concurrent access.

## API Reference

### Key Codes

Common key codes (using `vne::events::KeyCode` enum):

- `eSpace`, `eEnter`, `eEscape`, `eTab`
- `eA` through `eZ` for alphabet keys
- `e0` through `e9` for number keys
- `eUp`, `eDown`, `eLeft`, `eRight` for arrow keys
- `eF1` through `eF12` for function keys

### Mouse Button Codes

- `eMouseButtonLeft`, `eMouseButtonRight`, `eMouseButtonMiddle`
- `eMouseButton1` through `eMouseButton8` for additional buttons

### Gamepad Button Codes

- `eGamepadButtonA`, `eGamepadButtonB`, `eGamepadButtonX`, `eGamepadButtonY`
- `eGamepadButtonLeftBumper`, `eGamepadButtonRightBumper`
- `eGamepadButtonStart`, `eGamepadButtonBack`
- `eGamepadButtonDpadUp`, `eGamepadButtonDpadDown`, etc.

## Usage Examples

### Basic Input Polling

```cpp
#include <vertexnova/events/events.h>

void Update() {
    // Check keyboard input
    if (vne::events::Input::isKeyPressed(
            static_cast<int>(vne::events::KeyCode::eW))) {
        // Move forward (continuous)
    }

    if (vne::events::Input::isKeyJustPressed(
            static_cast<int>(vne::events::KeyCode::eSpace))) {
        // Jump (single press)
    }

    // Check mouse input
    if (vne::events::Input::isMouseButtonPressed(
            static_cast<int>(vne::events::MouseButton::eLeft))) {
        // Primary action (continuous)
    }

    // Get mouse position
    auto [mouse_x, mouse_y] = vne::events::Input::mousePosition();

    // Get mouse scroll
    auto [scroll_x, scroll_y] = vne::events::Input::mouseScroll();

    // Reset per-frame state
    vne::events::Input::nextFrame();
}
```

### Input State Management

```cpp
#include <vertexnova/events/input/input_manager.h>

// Get input manager instance
auto& input_manager = vne::events::InputManager::instance();
auto& input_state = input_manager.inputState();

// Update input state (typically done by window system)
input_state.updateKeyState(
    static_cast<int>(vne::events::KeyCode::eW), true);  // Key pressed
input_state.updateMousePosition(100, 200);

// Check state
if (input_state.isKeyPressed(
        static_cast<int>(vne::events::KeyCode::eW))) {
    // W key is currently pressed
}

// Reset for next frame
input_state.nextFrame();
```

### Platform Integration

```cpp
// In your window/event callback (e.g., GLFW)
void keyCallback(int key, int action) {
    bool pressed = (action == GLFW_PRESS || action == GLFW_REPEAT);
    vne::events::Input::updateKeyState(key, pressed);
}

void mouseButtonCallback(int button, int action) {
    bool pressed = (action == GLFW_PRESS);
    vne::events::Input::updateMouseButtonState(button, pressed);
}

void mousePositionCallback(double x, double y) {
    vne::events::Input::updateMousePosition(
        static_cast<int>(x), static_cast<int>(y));
}

// In your main loop
void mainLoop() {
    while (running) {
        // Process window events (updates input state)
        glfwPollEvents();
        
        // Update game logic (queries input state)
        update();
        
        // Reset per-frame input state
        vne::events::Input::nextFrame();
        
        render();
    }
}
```

## Performance Considerations

### Input Polling vs Event-Driven

- **Polling**: Good for continuous input (movement, camera)
- **Event-driven**: Good for discrete actions (jump, menu selection)
- **Hybrid**: Combine both approaches for optimal performance

### Per-Frame State Management

- Always call `nextFrame()` at the end of each frame
- Use `isKeyPressed()` for continuous input
- Use `isKeyJustPressed()` for single-press actions
- Use `isKeyJustReleased()` for release actions

### Thread Safety

- Input queries are thread-safe and can be called from any thread
- State updates should be synchronized (typically done by main thread)
- Uses read-write locks for efficient concurrent read access

## Best Practices

### Input Handling

- Use per-frame input checking for consistent behavior
- Handle multiple input devices (keyboard, gamepad, mouse)
- Implement input mapping for configurable controls
- Validate input parameters and handle edge cases

### Input State Management

- Centralize input state updates from window system
- Use RAII for automatic resource management
- Implement proper error handling for missing devices
- Cache frequently accessed input states

### Error Handling

- Validate key codes and button indices
- Handle missing input devices gracefully
- Provide fallback input methods
- Log input errors for debugging

## Troubleshooting

### Common Issues

#### Input Not Detected

- Check key codes match platform expectations
- Ensure input state is being updated by window system
- Verify `nextFrame()` is called each frame
- Check if input type is supported on current platform

#### Input Lag

- Increase input polling frequency
- Process input events immediately
- Ensure input state updates are synchronized
- Update input device drivers

#### Thread Safety Issues

- Use thread-safe input queries (all Input methods are thread-safe)
- Synchronize input state updates
- Use appropriate memory ordering for atomic operations

#### Platform Differences

- Handle platform-specific key code differences
- Check device support on target platform
- Adapt to platform-specific input event formats

## Related Documentation

- [Event System](../events/events.md) - Event-driven input handling
- [Input Architecture](diagrams/input.drawio) - Input system diagrams
- [API Documentation](../../README.md) - Full API reference

## Version History

- **v1.0.0**: Initial input system implementation

---

_This documentation is maintained with the codebase. For the latest information, see the source code and generated API documentation._
