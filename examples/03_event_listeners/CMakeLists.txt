#==============================================================================
# 03_event_listeners Example
#==============================================================================

add_executable(example_03_event_listeners
    main.cpp
    listener_demo.cpp
    listeners.cpp
)

target_link_libraries(example_03_event_listeners
    PRIVATE
        vne::events
        vne::logging
)

# Include common utilities directory
target_include_directories(example_03_event_listeners
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(example_03_event_listeners PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
