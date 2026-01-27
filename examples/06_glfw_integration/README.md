# GLFW Integration

Runs until **ESC** or **window close**; uses a `running` flag to stop the loop. Logs every event (resize, key, mouse) and logs **input polling** each frame.

## What it does

- **Game loop** keeps running while `running && !shouldClose()`.
- **`running`** is set to `false` on:
  - `WindowCloseEvent`, or
  - **ESC** key (via `KeyPressedEvent` for `eEscape`).
- **Event logs**: window resize, key pressed/released, mouse move, mouse button pressed/released.
- **Input polling log** each frame: W, A, S, D, Space, mouse position, left mouse button.

## Build and run

```bash
./scripts/build_macos.sh   # or build_linux.sh, etc.
./build/Debug/build-macos-clang-*/bin/examples/example_06_glfw_integration
```

## Log output

You’ll see lines like:

- `[Event] Window resized: 800x600`
- `[Event] Key pressed: 23`
- `[Event] Key released: 23`
- `[Event] Mouse moved: (100, 200)`
- `[Event] Mouse button pressed: 0`
- `[Input poll] W=1 A=0 S=0 D=0 Space=0 mouse=(100,200) LMB=0`

Exit with **ESC** or by closing the window.
