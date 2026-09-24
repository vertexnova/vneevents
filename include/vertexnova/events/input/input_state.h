#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Input state tracking for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "../export.h"
#include "../types.h"

#include <array>
#include <atomic>
#include <bit>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace vne::events {

class InputManager;

/**
 * @class InputState
 * @brief Lock-free keyboard, mouse, and window input snapshot.
 *
 * Host/DOM callbacks write; the render thread reads and calls nextFrame().
 * No mutex — locking the browser main thread behind a render worker stalls input.
 *
 * Coordinates and sizes use AtomicXY: both components live in one 64-bit atomic
 * so a reader never sees new-x with old-y (or vice versa).
 *
 * @threadsafe All public methods are thread-safe and lock-free.
 */
class VNEEVENTS_API InputState {
   public:
    static constexpr int kMaxKeyCode = static_cast<int>(KeyCode::eLast);
    static constexpr int kKeyCodeCount = kMaxKeyCode + 1;
    static constexpr int kMouseButtonCount = static_cast<int>(MouseButton::eLast) + 1;

    InputState();

    [[nodiscard]] bool isKeyPressed(int key) const;
    [[nodiscard]] bool isKeyJustPressed(int key) const;
    [[nodiscard]] bool isKeyJustReleased(int key) const;

    [[nodiscard]] bool isMouseButtonPressed(int button) const;
    [[nodiscard]] bool isMouseButtonJustPressed(int button) const;
    [[nodiscard]] bool isMouseButtonJustReleased(int button) const;

    [[nodiscard]] std::pair<int, int> mousePosition() const;
    [[nodiscard]] std::pair<float, float> mouseScroll() const;
    [[nodiscard]] std::pair<int, int> windowSize() const;

    void updateKeyState(int key, bool pressed);
    void updateMouseButtonState(int button, bool pressed);
    void updateMousePosition(int x, int y);
    void updateMouseScroll(float x_offset, float y_offset);
    void updateWindowSize(int width, int height);

    /// Clears just-pressed/released flags and scroll for the next frame.
    void nextFrame();

   private:
    /**
     * Tear-free (x, y) stored as one atomic word.
     *
     *   bits_ = [ x : high 32 ][ y : low 32 ]
     *
     * Two separate atomics would allow a reader to observe a mixed pair between
     * stores; one word means every load sees either the old pair or the new pair.
     */
    template<typename T>
    class AtomicXY {
        static_assert(sizeof(T) == sizeof(std::uint32_t));
        static_assert(std::is_same_v<T, int> || std::is_same_v<T, float>);

       public:
        void store(T x, T y) noexcept {
            bits_.store(pack(x, y), std::memory_order_relaxed);
        }

        [[nodiscard]] std::pair<T, T> load() const noexcept {
            return unpack(bits_.load(std::memory_order_relaxed));
        }

        void reset() noexcept { bits_.store(0, std::memory_order_relaxed); }

       private:
        std::atomic<std::uint64_t> bits_{0};

        static std::uint32_t toBits(T v) noexcept {
            if constexpr (std::is_same_v<T, float>) {
                return std::bit_cast<std::uint32_t>(v);
            } else {
                return static_cast<std::uint32_t>(static_cast<std::int32_t>(v));
            }
        }

        static T fromBits(std::uint32_t bits) noexcept {
            if constexpr (std::is_same_v<T, float>) {
                return std::bit_cast<float>(bits);
            } else {
                return static_cast<T>(static_cast<std::int32_t>(bits));
            }
        }

        static std::uint64_t pack(T x, T y) noexcept {
            return (static_cast<std::uint64_t>(toBits(x)) << 32U) | toBits(y);
        }

        static std::pair<T, T> unpack(std::uint64_t word) noexcept {
            return {fromBits(static_cast<std::uint32_t>(word >> 32U)),
                    fromBits(static_cast<std::uint32_t>(word))};
        }
    };

    using KeyFlags = std::array<std::atomic<bool>, kKeyCodeCount>;
    using ButtonFlags = std::array<std::atomic<bool>, kMouseButtonCount>;

    KeyFlags key_state_{};
    KeyFlags key_just_pressed_{};
    KeyFlags key_just_released_{};

    ButtonFlags mouse_button_state_{};
    ButtonFlags mouse_button_just_pressed_{};
    ButtonFlags mouse_button_just_released_{};

    AtomicXY<int> mouse_position_;
    AtomicXY<float> mouse_scroll_;
    AtomicXY<int> window_size_;

    friend class InputManager;
};

}  // namespace vne::events
