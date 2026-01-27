#==============================================================================
# 05_game_loop_integration Example
#==============================================================================

add_executable(example_05_game_loop_integration
    main.cpp
    game_loop_demo.cpp
    listeners.cpp
)

target_link_libraries(example_05_game_loop_integration
    PRIVATE
        vne::events
        vne::logging
)

# Include common utilities directory
target_include_directories(example_05_game_loop_integration
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(example_05_game_loop_integration PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
