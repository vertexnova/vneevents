#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Keyboard event classes for VertexNova Events.
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "types.h"

#include <sstream>
#include <cstdint>

namespace vne::events {

/**
 * @class KeyEvent
 * @brief Base class for keyboard events.
 */
class VNEEVENTS_API KeyEvent : public Event {
   public:
    [[nodiscard]] KeyCode keyCode() const noexcept { return key_code_; }
    [[nodiscard]] uint8_t modifiers() const noexcept { return modifiers_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eKeyboard | EventCategory::eInput; }

   protected:
    KeyEvent(EventType event_type, KeyCode key_code, uint8_t modifiers = 0)
        : Event(event_type)
        , key_code_(key_code)
        , modifiers_(modifiers) {}

   private:
    KeyCode key_code_;
    uint8_t modifiers_;
};

/**
 * @class KeyPressedEvent
 * @brief Event generated when a key is pressed.
 */
class VNEEVENTS_API KeyPressedEvent : public KeyEvent {
   public:
    explicit KeyPressedEvent(KeyCode key_code, uint8_t modifiers = 0)
        : KeyEvent(EventType::eKeyPressed, key_code, modifiers) {}

    [[nodiscard]] std::string name() const override { return "KeyPressed"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyPressedEvent: " << static_cast<int>(keyCode());
        return ss.str();
    }
};

/**
 * @class KeyReleasedEvent
 * @brief Event generated when a key is released.
 */
class VNEEVENTS_API KeyReleasedEvent : public KeyEvent {
   public:
    explicit KeyReleasedEvent(KeyCode key_code, uint8_t modifiers = 0)
        : KeyEvent(EventType::eKeyReleased, key_code, modifiers) {}

    [[nodiscard]] std::string name() const override { return "KeyReleased"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyReleasedEvent: " << static_cast<int>(keyCode());
        return ss.str();
    }
};

/**
 * @class KeyRepeatEvent
 * @brief Event generated when a key is held and repeats.
 */
class VNEEVENTS_API KeyRepeatEvent : public KeyEvent {
   public:
    KeyRepeatEvent(KeyCode key_code, uint32_t repeat_count)
        : KeyEvent(EventType::eKeyRepeat, key_code)
        , repeat_count_(repeat_count) {}

    [[nodiscard]] uint32_t repeatCount() const noexcept { return repeat_count_; }

    [[nodiscard]] std::string name() const override { return "KeyRepeat"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyRepeatEvent: " << static_cast<int>(keyCode()) << " (" << repeat_count_ << ")";
        return ss.str();
    }

   private:
    uint32_t repeat_count_;
};

/**
 * @class KeyTypedEvent
 * @brief Event generated when a character is typed.
 */
class VNEEVENTS_API KeyTypedEvent : public KeyEvent {
   public:
    explicit KeyTypedEvent(KeyCode key_code)
        : KeyEvent(EventType::eKeyTyped, key_code) {}

    [[nodiscard]] std::string name() const override { return "KeyTyped"; }

    [[nodiscard]] std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyTypedEvent: " << static_cast<int>(keyCode());
        return ss.str();
    }
};

}  // namespace vne::events
