#==============================================================================
# 02_input_polling Example
#==============================================================================

add_executable(example_02_input_polling
    main.cpp
    input_demo.cpp
)

target_link_libraries(example_02_input_polling
    PRIVATE
        vne::events
        vne::logging
)

# Include common utilities directory
target_include_directories(example_02_input_polling
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(example_02_input_polling PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
