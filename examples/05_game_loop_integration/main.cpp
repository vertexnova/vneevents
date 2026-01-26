/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Example: Game Loop Integration
 * Demonstrates complete game loop with event processing and input polling
 * ----------------------------------------------------------------------
 */

#include "game_loop_demo.h"

#include "common/logging_guard.h"

#include <vertexnova/logging/logging.h>

using vne::events::examples::GameLoopDemo;
using vne::events::examples::LoggingGuard_C;

int main() {
    // Initialize logging for console output
    LoggingGuard_C logging_guard;

    VNE_LOG_INFO << "VneEvents Example: Game Loop Integration";
    VNE_LOG_INFO << "========================================";
    VNE_LOG_INFO << "";

    // Create and run the game loop demonstration
    GameLoopDemo demo;
    demo.run();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Done ===";
    return 0;
}
