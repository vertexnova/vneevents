#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Event listener classes for Game Loop Integration example
 * ----------------------------------------------------------------------
 */

#include <vertexnova/events/events.h>

namespace vne::events::examples {

/**
 * @class GameEventListener
 * @brief Listener that handles game events (window close, resize, key press).
 */
class GameEventListener : public vne::events::EventListener {
   public:
    explicit GameEventListener(bool* running);
    ~GameEventListener() override = default;

    void onEvent(const vne::events::Event& event) override;

   private:
    bool* running_;
};

}  // namespace vne::events::examples
