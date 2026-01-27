#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener classes for Multiple Listeners example
 * ----------------------------------------------------------------------
 */

#include <vertexnova/events/events.h>

#include <string>

namespace vne::events::examples {

/**
 * @class AudioListener
 * @brief Listener that handles audio feedback for key events.
 */
class AudioListener : public vne::events::EventListener {
   public:
    AudioListener() = default;
    ~AudioListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class UIListener
 * @brief Listener that handles UI updates for key events.
 */
class UIListener : public vne::events::EventListener {
   public:
    UIListener() = default;
    ~UIListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class GameplayListener
 * @brief Listener that handles gameplay logic for key events.
 */
class GameplayListener : public vne::events::EventListener {
   public:
    GameplayListener() = default;
    ~GameplayListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class InputListener
 * @brief Listener for input subsystem events.
 */
class InputListener : public vne::events::EventListener {
   public:
    InputListener() = default;
    ~InputListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class WindowListener
 * @brief Listener for window subsystem events.
 */
class WindowListener : public vne::events::EventListener {
   public:
    WindowListener() = default;
    ~WindowListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class RenderListener
 * @brief Listener for render subsystem events.
 */
class RenderListener : public vne::events::EventListener {
   public:
    RenderListener() = default;
    ~RenderListener() override = default;

    void onEvent(const vne::events::Event& event) override;
};

/**
 * @class TemporaryListener
 * @brief Temporary listener for demonstrating lifecycle management.
 */
class TemporaryListener : public vne::events::EventListener {
   public:
    explicit TemporaryListener(const std::string& name);
    ~TemporaryListener() override = default;

    void onEvent(const vne::events::Event& event) override;

   private:
    std::string name_;
};

}  // namespace vne::events::examples
