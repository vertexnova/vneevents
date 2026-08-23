#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   August 2026
 *
 * Text input event for committed UTF-8 from the platform character/IME path.
 * ----------------------------------------------------------------------
 */

#include "event.h"
#include "types.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

namespace vne::events {

/**
 * @brief Encodes a single Unicode code point as UTF-8, for producers that receive code points.
 *
 * Platform character callbacks hand over a code point (GLFW's char callback, Win32 WM_CHAR after
 * surrogate reassembly), while TextInputEvent carries UTF-8. This is the bridge, so no producer
 * hand-rolls the encoding.
 *
 * @param code_point The code point to encode.
 * @return Its UTF-8 encoding, or an empty string if the value is out of range or a surrogate half.
 */
[[nodiscard]] inline std::string utf8FromCodePoint(char32_t code_point) {
    const auto cp = static_cast<std::uint32_t>(code_point);
    std::string out;
    if (cp > 0x10FFFFU || (cp >= 0xD800U && cp <= 0xDFFFU)) {
        return out;
    }
    if (cp < 0x80U) {
        // 1-byte ASCII (U+0000..U+007F)
        out += static_cast<char>(cp);
    } else if (cp < 0x800U) {
        // 2-byte sequence (U+0080..U+07FF)
        out += static_cast<char>(0xC0U | (cp >> 6U));
        out += static_cast<char>(0x80U | (cp & 0x3FU));
    } else if (cp < 0x10000U) {
        // 3-byte sequence (U+0800..U+FFFF, excluding surrogates)
        out += static_cast<char>(0xE0U | (cp >> 12U));
        out += static_cast<char>(0x80U | ((cp >> 6U) & 0x3FU));
        out += static_cast<char>(0x80U | (cp & 0x3FU));
    } else {
        // 4-byte sequence (U+10000..U+10FFFF)
        out += static_cast<char>(0xF0U | (cp >> 18U));
        out += static_cast<char>(0x80U | ((cp >> 12U) & 0x3FU));
        out += static_cast<char>(0x80U | ((cp >> 6U) & 0x3FU));
        out += static_cast<char>(0x80U | (cp & 0x3FU));
    }
    return out;
}

/**
 * @class TextInputEvent
 * @brief Committed text produced by the platform character-input / IME path.
 *
 * Distinct from KeyPressedEvent, which identifies a physical key rather than the characters it
 * produced: one keypress may commit several characters, or none.
 * text() is UTF-8 and may hold more than one code point per event (IME composition commit,
 * dead-key composition, emoji, surrogate pairs reassembled by the producer). This is the event
 * to feed a text field; use KeyPressedEvent for shortcuts and navigation.
 *
 * @throws std::invalid_argument if @p utf8_text is empty.
 */
class VNEEVENTS_API TextInputEvent : public Event {
   public:
    explicit TextInputEvent(std::string utf8_text, WindowId window_id = kInvalidWindowId)
        : Event(EventType::eTextInput, window_id)
        , text_(std::move(utf8_text)) {
        if (text_.empty()) {
            throw std::invalid_argument("TextInputEvent: utf8_text must not be empty");
        }
    }

    /// The committed text, UTF-8 encoded. Never empty.
    [[nodiscard]] const std::string& text() const noexcept { return text_; }

    [[nodiscard]] int categoryFlags() const override { return EventCategory::eKeyboard | EventCategory::eInput; }

    [[nodiscard]] std::string name() const override { return "TextInput"; }

    [[nodiscard]] std::string toString() const override { return "TextInputEvent: \"" + text_ + "\"" + windowSuffix(); }

   private:
    std::string text_;
};

}  // namespace vne::events
