# GLFW Integration

Runs until **ESC** or **window close**; uses a `running` flag to stop the loop. Logs every event (resize, key, mouse, **touch**, **modifiers**) and logs **input polling** each frame.

## What it does

- **Game loop** keeps running while `running && !shouldClose()`.
- **`running`** is set to `false` on:
  - `WindowCloseEvent`, or
  - **ESC** key (via `KeyPressedEvent` for `eEscape`).
- **Event logs**: window resize, key pressed/released, mouse move, mouse button pressed/released, **touch** (press/move/release), and **modifiers** (Shift/Ctrl/Alt/Super) on key and mouse events.
- **Input polling log** each frame: W, A, S, D, Space, mouse position, left mouse button.

## How to test touch and modifiers

- **Touch**: Left mouse button is **mimicked as touch** (touch id `0`). Click/drag with LMB to see `[Event] Touch press`, `Touch move`, `Touch release` in the log. This lets you exercise touch events on desktop.
- **Modifiers + key**: Hold **Shift**, **Ctrl**, **Alt**, or **Super** (Win/Cmd) and press or release a key. The log shows e.g. `Key pressed: 87 mods=Shift` or `mods=Ctrl|Alt`.
- **Modifiers + mouse**: Hold a modifier and move the mouse or press/release a button. You’ll see `Mouse moved: (x, y) mods=…` and `Mouse button pressed/released … mods=…`.

## Build and run

```bash
./scripts/build_macos.sh   # or build_linux.sh, etc.
./build/Debug/build-macos-clang-*/bin/examples/example_06_glfw_integration
```

## Log output

You’ll see lines like:

- `[Event] Window resized: 800x600`
- `[Event] Key pressed: 87 mods=Shift` (87 = W)
- `[Event] Key released: 87 mods=none`
- `[Event] Mouse moved: (100, 200) mods=Ctrl`
- `[Event] Mouse button pressed: 0 mods=Alt`
- `[Event] Touch press: id=0 (100, 200)` (when you press LMB)
- `[Event] Touch move: id=0 (105, 205)` (when you drag with LMB)
- `[Event] Touch release: id=0 (110, 210)` (when you release LMB)
- `[Input poll] W=1 A=0 S=0 D=0 Space=0 mouse=(100,200) LMB=0`

Exit with **ESC** or by closing the window.
