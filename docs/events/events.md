# Event System

## Overview

The Event System provides a comprehensive event-driven architecture for VertexNova Events library. It enables loose coupling between components through a publish-subscribe pattern, supporting various event types including window, input, mouse, keyboard, and touch events.

## Architecture

The Event System follows a layered architecture with clear separation of concerns:

```
Event System
├── Event              # Base event class
├── Event Types        # Specific event implementations
│   ├── WindowEvent    # Window-related events
│   ├── KeyEvent       # Keyboard events
│   ├── MouseEvent     # Mouse events
│   └── TouchEvent     # Touch events
├── Event Management   # Event handling infrastructure
│   ├── EventBus       # Central event routing
│   ├── EventDispatcher # Event distribution
│   ├── EventQueue     # Thread-safe queuing
│   └── EventManager   # High-level event management
└── Event Listeners    # Event subscription system
```

### Key Design Principles

- **Type Safety**: Strongly typed events with compile-time checking
- **Thread Safety**: Safe concurrent event processing
- **Performance**: Efficient event routing and filtering
- **Extensibility**: Easy to add new event types
- **Memory Safety**: RAII-based event lifecycle management

## Core Components

### Event - Base Event Class

The foundation of the event system, providing a common interface for all events.

**Key Features:**

- **Category Flags**: Events can belong to multiple categories for efficient filtering
- **Timestamp**: Automatic timestamp generation for event ordering
- **Handled State**: Track whether events have been processed
- **String Representation**: Human-readable event descriptions

### Event Types

- **Window Events**: Window resize, close, focus, etc.
- **Key Events**: Key press, release, repeat
- **Mouse Events**: Mouse move, button press/release, scroll
- **Touch Events**: Touch press, move, release

### Event Management

- **Event Bus**: Central routing system for event distribution
- **Event Dispatcher**: Handles event distribution to registered listeners
- **Event Queue**: Thread-safe event queuing for asynchronous processing
- **Event Manager**: High-level event management and coordination

## API Reference

### Event Categories

Events can belong to multiple categories for efficient filtering:

- `eApplication`: Application-level events
- `eInput`: General input events
- `eKeyboard`: Keyboard-specific events
- `eMouse`: Mouse-specific events
- `eWindow`: Window-related events
- `eTouch`: Touch input events

### Event Types

Common event types include:

- `eWindowClose`, `eWindowResize`, `eWindowFocus`
- `eKeyPressed`, `eKeyReleased`, `eKeyTyped`
- `eMouseButtonPressed`, `eMouseButtonReleased`, `eMouseButtonDoubleClicked`, `eMouseMoved`, `eMouseScrolled`
- `eTouchPress`, `eTouchMove`, `eTouchRelease`

#### Event classes and `EventType`

| Event class | EventType | Category |
|-------------|-----------|----------|
| `KeyPressedEvent` | `eKeyPressed` | Keyboard, Input |
| `KeyReleasedEvent` | `eKeyReleased` | Keyboard, Input |
| `KeyRepeatEvent` | `eKeyRepeat` | Keyboard, Input |
| `KeyTypedEvent` | `eKeyTyped` | Keyboard, Input |
| `MouseButtonPressedEvent` | `eMouseButtonPressed` | MouseButton, Input |
| `MouseButtonReleasedEvent` | `eMouseButtonReleased` | MouseButton, Input |
| `MouseButtonDoubleClickedEvent` | `eMouseButtonDoubleClicked` | MouseButton, Input |
| `MouseMovedEvent` | `eMouseMoved` | Mouse, Input |
| `MouseScrolledEvent` | `eMouseScrolled` | Mouse, Input |
| `WindowCloseEvent` | `eWindowClose` | Window |
| `WindowResizeEvent` | `eWindowResize` | Window |
| `WindowFocusEvent` | `eWindowFocus` | Window |
| `TouchPressEvent` | `eTouchPress` | TouchScreen, Input |
| `TouchReleaseEvent` | `eTouchRelease` | TouchScreen, Input |
| `TouchMoveEvent` | `eTouchMove` | TouchScreen, Input |

For `WindowFocusEvent`, use `focused()` (or `toString()`) to distinguish focus gained vs lost; `name()` is `"WindowFocus"` for both.

## Usage Examples

### Basic Event Handling

```cpp
#include <vertexnova/events/events.h>

// Create custom event
class CustomEvent : public vne::events::Event {
public:
    CustomEvent() : Event(vne::events::EventType::eCustom) {}
    
    [[nodiscard]] int categoryFlags() const override {
        return vne::events::EventCategory::eApplication;
    }
    
    [[nodiscard]] std::string toString() const override {
        return "CustomEvent";
    }
};

// Handle events
class MyEventListener : public vne::events::EventListener {
public:
    void onEvent(const vne::events::Event& event) override {
        // Process event based on type and category
        switch (event.type()) {
            case vne::events::EventType::eKeyPressed:
                // Handle key press
                break;
            case vne::events::EventType::eWindowResize:
                // Handle window resize
                break;
            default:
                break;
        }
    }
};

// Register listener and dispatch events
auto& manager = vne::events::EventManager::instance();
auto listener = std::make_shared<MyEventListener>();

manager.registerListener(vne::events::EventType::eKeyPressed, listener);
manager.pushEvent(std::make_unique<vne::events::KeyPressedEvent>(
    static_cast<int>(vne::events::KeyCode::eA)));
manager.processEvents();
```

### Event Filtering

```cpp
// Filter by category for efficiency
if (event.categoryFlags() & vne::events::EventCategory::eInput) {
    // Handle input events only
}

// Filter by specific event type
switch (event.type()) {
    case vne::events::EventType::eKeyPressed:
        // Handle key press
        break;
    case vne::events::EventType::eWindowResize:
        // Handle window resize
        break;
    default:
        break;
}
```

### Using Event Queue

```cpp
#include <vertexnova/events/event_queue.h>

vne::events::EventQueue queue;

// Push events to queue
queue.push(std::make_unique<vne::events::KeyPressedEvent>(
    static_cast<int>(vne::events::KeyCode::eSpace)));

// Process events from queue
while (!queue.isEmpty()) {
    auto event = queue.pop();
    if (event) {
        // Process event
        processEvent(*event);
    }
}
```

### Using Event Bus

```cpp
#include <vertexnova/events/event_bus.h>

vne::events::EventBus bus;
auto listener = std::make_shared<MyEventListener>();

// Subscribe to events
bus.subscribe(vne::events::EventType::eKeyPressed, listener);

// Publish events
bus.publish(std::make_unique<vne::events::KeyPressedEvent>(
    static_cast<int>(vne::events::KeyCode::eEnter)));

// Process all queued events
bus.processEvents();
```

## Performance Considerations

### Event Filtering

- Use category flags for efficient event filtering
- Filter by event type for specific handling
- Avoid processing unnecessary events

### Event Pooling

- Reuse event objects for high-frequency events
- Implement event pooling to reduce allocation overhead
- Use RAII for automatic event cleanup

### Batch Processing

- Process multiple events in batches for better performance
- Use thread-safe event queues for concurrent processing
- Minimize event object copying

## Best Practices

### Event Design

- Use meaningful event names and categories
- Mark events as handled when processed
- Keep event data minimal and focused

### Event Handling

- Use category filtering for efficiency
- Implement proper error handling
- Use RAII for event management

### Thread Safety

- Use thread-safe event queues
- Ensure proper synchronization
- Avoid blocking in event handlers

## Troubleshooting

### Common Issues

#### Event Not Received

- Check event listener registration
- Verify event categories match filters
- Ensure event type is being handled
- Verify `processEvents()` is being called

#### Performance Issues

- Implement category-based filtering
- Use event pooling for high-frequency events
- Process events in batches
- Consider using event queues for deferred processing

#### Memory Issues

- Use RAII for automatic cleanup
- Implement event pooling
- Avoid event object copying
- Use smart pointers for event ownership

## Related Documentation

- [Input System](../input/input.md) - Input event handling and polling
- [Event Architecture](diagrams/events.drawio) - Event system diagrams
- [API Documentation](../../README.md) - Full API reference

## Version History

- **v1.0.0**: Initial event system implementation

---

_This documentation is maintained with the codebase. For the latest information, see the source code and generated API documentation._
