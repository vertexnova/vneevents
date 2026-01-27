#==============================================================================
# 04_multithreaded_events Example
#==============================================================================

add_executable(example_04_multithreaded_events
    main.cpp
    threaded_demo.cpp
    listeners.cpp
)

target_link_libraries(example_04_multithreaded_events
    PRIVATE
        vne::events
        vne::logging
)

# Include common utilities directory
target_include_directories(example_04_multithreaded_events
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(example_04_multithreaded_events PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
