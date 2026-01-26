/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 * ----------------------------------------------------------------------
 */

// Testing framework
#include <gtest/gtest.h>

// Project headers
#include <vertexnova/logging/logging.h>

namespace {

/**
 * @class LoggingGuard
 * @brief RAII guard for logging configuration in tests.
 *
 * This class initializes the logging system with console-only output
 * in its constructor and shuts it down in its destructor. This ensures
 * proper logging setup and cleanup for the test suite.
 */
class LoggingGuard {
   public:
    LoggingGuard() {
        // Configure logging for tests - console only
        vne::log::LoggerConfig config;
        config.name = vne::log::kDefaultLoggerName;
        config.sink = vne::log::LogSinkType::eConsole;
        config.console_pattern = "[%l] [%n] %v";
        config.log_level = vne::log::LogLevel::eWarn;  // Only show warnings and above in tests
        config.async = false;

        vne::log::Logging::configureLogger(config);
    }

    ~LoggingGuard() { vne::log::Logging::shutdown(); }

    // Non-copyable
    LoggingGuard(const LoggingGuard&) = delete;
    LoggingGuard& operator=(const LoggingGuard&) = delete;
};

}  // namespace

int main(int argc, char** argv) {
    // Initialize logging before running tests
    LoggingGuard logging_guard;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
