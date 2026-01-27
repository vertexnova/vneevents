/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Example: GLFW Integration – event logging, input polling, run until ESC/close
 * ----------------------------------------------------------------------
 */

#include "glfw_integration_demo.h"
#include "common/logging_guard.h"
#include <vertexnova/logging/logging.h>

namespace {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.glfw_integration")
}

using vne::events::examples::GlfwIntegrationDemo;
using vne::events::examples::LoggingGuard_C;

int main() {
    LoggingGuard_C logging_guard;

    VNE_LOG_INFO << "VneEvents Example: GLFW Integration";
    VNE_LOG_INFO << "====================================";
    VNE_LOG_INFO << "";

    GlfwIntegrationDemo demo;
    demo.run();

    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Done ===";
    return 0;
}
