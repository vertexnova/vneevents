/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Example: Input Polling
 * Demonstrates input state polling in a game loop
 * ----------------------------------------------------------------------
 */

#include "input_demo.h"

#include "common/logging_guard.h"

#include <vertexnova/logging/logging.h>

namespace {
    CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.input_polling")
}

using vne::events::examples::InputDemo;
using vne::events::examples::LoggingGuard_C;

int main() {
    // Initialize logging for console output
    LoggingGuard_C logging_guard;

    VNE_LOG_INFO << "VneEvents Example: Input Polling";
    VNE_LOG_INFO << "================================";
    VNE_LOG_INFO << "";

    // Create and run the input demonstration
    InputDemo demo;
    demo.run();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Done ===";
    return 0;
}
