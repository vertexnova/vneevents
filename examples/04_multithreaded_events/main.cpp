/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Example: Multithreaded Events
 * Demonstrates thread-safe event processing
 * ----------------------------------------------------------------------
 */

#include "threaded_demo.h"

#include "common/logging_guard.h"

#include <vertexnova/logging/logging.h>

namespace {
    CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.multithreaded_events")
}

using vne::events::examples::LoggingGuard_C;
using vne::events::examples::ThreadedDemo;

int main() {
    // Initialize logging for console output
    LoggingGuard_C logging_guard;

    VNE_LOG_INFO << "VneEvents Example: Multithreaded Events";
    VNE_LOG_INFO << "=======================================";
    VNE_LOG_INFO << "";

    // Create and run the multithreaded demonstration
    ThreadedDemo demo;
    demo.run();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Done ===";
    return 0;
}
