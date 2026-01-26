# 02 - Input Polling

This example demonstrates input state polling using the `Input` static interface.

## What This Example Shows

1. **Keyboard Polling**: Querying keyboard state with `isKeyPressed()` and `isKeyJustPressed()`
2. **Mouse Polling**: Querying mouse button state and position
3. **Per-Frame State**: Using `nextFrame()` to reset just-pressed/released states
4. **Input State Management**: Understanding the difference between pressed and just-pressed states

## Building

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build .
```

## Running

```bash
./bin/examples/example_02_input_polling
```

## Expected Output

```
[INFO] [vneevents.examples] VneEvents Example: Input Polling
[INFO] [vneevents.examples] ================================
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Input Polling Demonstration ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Keyboard Polling ---
[INFO] [vneevents.examples]   Simulating key presses...
[INFO] [vneevents.examples]   W key pressed
[INFO] [vneevents.examples]     isKeyPressed(W): true
[INFO] [vneevents.examples]     isKeyJustPressed(W): true
[INFO] [vneevents.examples]   A key pressed
[INFO] [vneevents.examples]     isKeyPressed(A): true
[INFO] [vneevents.examples]     isKeyJustPressed(A): true
[INFO] [vneevents.examples]   W key released
[INFO] [vneevents.examples]     isKeyPressed(W): false
[INFO] [vneevents.examples]     isKeyJustReleased(W): true
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Mouse Polling ---
[INFO] [vneevents.examples]   Simulating mouse input...
[INFO] [vneevents.examples]   Mouse position: (100, 200)
[INFO] [vneevents.examples]   Left mouse button pressed
[INFO] [vneevents.examples]     isMouseButtonPressed(Left): true
[INFO] [vneevents.examples]     isMouseButtonJustPressed(Left): true
[INFO] [vneevents.examples]   Mouse moved to: (150, 250)
[INFO] [vneevents.examples]   Mouse scrolled: (0, 1)
[INFO] [vneevents.examples]   Left mouse button released
[INFO] [vneevents.examples]     isMouseButtonPressed(Left): false
[INFO] [vneevents.examples]     isMouseButtonJustReleased(Left): true
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] --- Per-Frame State Management ---
[INFO] [vneevents.examples]   Frame 1:
[INFO] [vneevents.examples]     Space key pressed
[INFO] [vneevents.examples]       isKeyJustPressed(Space): true
[INFO] [vneevents.examples]       isKeyPressed(Space): true
[INFO] [vneevents.examples]     After nextFrame():
[INFO] [vneevents.examples]       isKeyJustPressed(Space): false
[INFO] [vneevents.examples]       isKeyPressed(Space): true
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples]   Frame 2:
[INFO] [vneevents.examples]     Space key still held
[INFO] [vneevents.examples]       isKeyJustPressed(Space): false
[INFO] [vneevents.examples]       isKeyPressed(Space): true
[INFO] [vneevents.examples]     Space key released
[INFO] [vneevents.examples]       isKeyJustReleased(Space): true
[INFO] [vneevents.examples]     After nextFrame():
[INFO] [vneevents.examples]       isKeyJustReleased(Space): false
[INFO] [vneevents.examples]       isKeyPressed(Space): false
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Demonstration Complete ===
[INFO] [vneevents.examples] 
[INFO] [vneevents.examples] === Done ===
```

## Key Concepts

- **Input::isKeyPressed()**: Returns true if key is currently pressed (held down)
- **Input::isKeyJustPressed()**: Returns true only on the first frame the key was pressed
- **Input::isKeyJustReleased()**: Returns true only on the first frame the key was released
- **Input::nextFrame()**: Must be called once per frame to reset just-pressed/released states
- **Input State Updates**: Use `Input::updateKeyState()` and `Input::updateMouseButtonState()` to update state (typically done by platform callbacks)

## Code Structure

- `main.cpp`: Minimal setup, uses `LoggingGuard_C` and calls `InputDemo::run()`
- `input_demo.h` / `input_demo.cpp`: Helper class encapsulating the demonstration logic
- `common/logging_guard.h`: Shared logging configuration for all examples
