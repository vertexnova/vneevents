#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Common logging configuration for VneEvents examples
 * ----------------------------------------------------------------------
 */

#include <vertexnova/logging/logging.h>

namespace vne::events::examples {

/**
 * @class LoggingGuard_C
 * @brief RAII guard for console logging configuration in examples.
 *
 * This class initializes the logging system with console output
 * in its constructor and shuts it down in its destructor. Use this
 * at the start of main() in example programs.
 *
 * Usage:
 * @code
 * int main() {
 *     LoggingGuard_C logging_guard;
 *     // ... example code ...
 *     return 0;
 * }
 * @endcode
 */
class LoggingGuard_C {
   public:
    LoggingGuard_C() {
        vne::log::LoggerConfig config;
        config.name = vne::log::kDefaultLoggerName;
        config.sink = vne::log::LogSinkType::eConsole;
        config.console_pattern = "[%l] [%n] %v";
        config.log_level = vne::log::LogLevel::eInfo;
        config.async = false;

        vne::log::Logging::configureLogger(config);
    }

    ~LoggingGuard_C() { vne::log::Logging::shutdown(); }

    // Non-copyable
    LoggingGuard_C(const LoggingGuard_C&) = delete;
    LoggingGuard_C& operator=(const LoggingGuard_C&) = delete;
};

}  // namespace vne::events::examples
