# Platform Integration Example

This example demonstrates how to integrate VneEvents with platform windowing systems, specifically GLFW. It creates an actual GLFW window with OpenGL 3.3 context and demonstrates real-time event translation.

## Overview

This example shows:
- **Real GLFW window** with OpenGL 3.3 Core Profile context
- **GLAD OpenGL loader** - cross-platform OpenGL function loading
- **GLFW callback integration** - actual callbacks bound to GLFW window
- **Platform event translation** (GLFW -> VneEvents) in real-time
- **Input state updates** from platform callbacks
- **Game loop** with event processing and rendering

## Key Concepts

### Event Translation Layer

The `GLFWIntegration` class demonstrates how to translate platform-specific events (GLFW) into the abstract VneEvents format:

- **GLFW Key Codes** → `KeyCode` enum
- **GLFW Mouse Buttons** → `MouseButton` enum
- **GLFW Window Events** → `WindowResizeEvent`, `WindowCloseEvent`

### Dual Input System

The integration shows how to use both event-driven and polling approaches:

1. **Event-driven**: GLFW callbacks → VneEvents → Event listeners
2. **Polling**: GLFW callbacks → Input state updates → `Input::isKeyPressed()`

### Integration Pattern

```cpp
// In GLFW callback:
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GLFWIntegration* integration = static_cast<GLFWIntegration*>(
        glfwGetWindowUserPointer(window));
    
    if (action == GLFW_PRESS) {
        integration->simulateKeyPress(key);
    } else if (action == GLFW_RELEASE) {
        integration->simulateKeyRelease(key);
    }
}
```

## Building

This example requires:
- **GLFW** (included as git submodule at `external/glfw`)
- **GLAD** (OpenGL loader library, located at `/Users/ayadav/workspace/github/vertexnova/3rd_party/glad`)

```bash
cmake -B build -DBUILD_EXAMPLES=ON
cmake --build build
```

**Note**: GLAD is referenced from the shared VertexNova 3rd_party location. The build system will automatically detect it.

## Running

```bash
./build/bin/examples/example_06_platform_integration
```

## Expected Output

When you run the example, a window will open. You'll see output like:

```
VneEvents Example: Platform Integration
========================================

=== Platform Integration Demonstration ===

--- GLFW Integration ---
  [GLFW] Window created: 800x600
  [GLFW] OpenGL 3.3 Core Profile context created
  [GLFW] Callbacks registered
  [GLAD] OpenGL version: 3.3 ...
  [GLAD] OpenGL vendor: ...
  [GLAD] OpenGL renderer: ...

  Window is now open. Try:
    - Press keys (W, A, S, D, Space, ESC)
    - Move mouse
    - Click mouse buttons
    - Resize window
    - Press ESC to close

  [Callback] Key pressed: 87 -> KeyCode::23
  [Callback] Mouse button pressed: 0 -> MouseButton::0
  [Callback] Window resized: 1920x1080
  ...

--- Event Translation ---
  Event translation is demonstrated in the GLFW callbacks:
    - GLFW_KEY_W -> KeyPressedEvent(KeyCode::eW)
    - GLFW_MOUSE_BUTTON_LEFT -> MouseButtonPressedEvent(MouseButton::eLeft)
    - GLFW window resize -> WindowResizeEvent(width, height)

  All events are automatically translated when you interact with the window.

=== Demonstration Complete ===
```

## Real-World Usage

In a real application, you would:

1. Initialize GLFW and create a window with OpenGL context
2. Initialize GLAD using `gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)`
3. Create a `GLFWIntegration` instance
4. GLFW callbacks are automatically set up by `GLFWIntegration`
5. In your game loop:
   - Call `glfwPollEvents()` to trigger callbacks
   - Call `EventManager::processEvents()` to handle events
   - Use `Input::isKeyPressed()` for continuous input
   - Render using OpenGL functions (loaded via GLAD)
   - Call `glfwSwapBuffers()` to present the frame
   - Call `Input::nextFrame()` at the end of each frame

## Key Implementation Details

### GLAD Integration

GLAD is used instead of platform-specific OpenGL headers:
- **Cross-platform**: Works on Windows, macOS, Linux
- **Function loading**: Dynamically loads OpenGL functions at runtime
- **No platform-specific includes**: No need for `#ifdef __APPLE__` etc.

```cpp
// Include GLAD before any OpenGL calls
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// After creating OpenGL context:
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
```

### Event Translation

The `GLFWIntegration` class translates GLFW events to VneEvents:
- **Key events**: `GLFW_KEY_*` → `KeyCode` enum
- **Mouse events**: `GLFW_MOUSE_BUTTON_*` → `MouseButton` enum
- **Window events**: GLFW window callbacks → `WindowResizeEvent`, `WindowCloseEvent`

## Notes

- This example creates an **actual GLFW window** (not simulated)
- GLAD is referenced from the shared VertexNova 3rd_party location
- A full implementation would include all GLFW key/button mappings
- The translation layer can be extended for other platforms (SDL, Win32, Cocoa, etc.)
- The same pattern works for iOS (UIKit) and Web (DOM events), but this example is desktop-only
