#==============================================================================
# 01_hello_events Example
#==============================================================================

add_executable(example_01_hello_events
    main.cpp
    event_demo.cpp
    listeners.cpp
)

target_link_libraries(example_01_hello_events
    PRIVATE
        vne::events
        vne::logging
)

# Include common utilities directory
target_include_directories(example_01_hello_events
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(example_01_hello_events PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
