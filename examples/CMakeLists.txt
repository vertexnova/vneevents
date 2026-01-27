#==============================================================================
# VneEvents Examples CMakeLists.txt
#==============================================================================

# Include common utilities directory (for include paths)
add_subdirectory(common)

# 01 - Hello Events: Basic event system usage
add_subdirectory(01_hello_events)

# 02 - Input Polling: Input state polling in a game loop
add_subdirectory(02_input_polling)

# 03 - Event Listeners: Multiple listeners and subsystem patterns
add_subdirectory(03_event_listeners)

# 04 - Multithreaded Events: Thread-safe event processing
add_subdirectory(04_multithreaded_events)

# 05 - Game Loop Integration: Complete game loop with event processing and input polling
add_subdirectory(05_game_loop_integration)
# 06 - GLFW Integration: GLFW + OpenGL 3.3, event logging, input polling (desktop only)
if(NOT EMSCRIPTEN AND NOT (APPLE AND (IOS OR ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")))
    add_subdirectory(06_glfw_integration)
endif()
