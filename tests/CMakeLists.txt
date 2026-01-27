#==============================================================================
# VertexNova Events - Tests
#==============================================================================

# Use Google Test from submodule
if(EXISTS "${PROJECT_SOURCE_DIR}/external/googletest/CMakeLists.txt")
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/googletest ${CMAKE_BINARY_DIR}/external/googletest)
    set(GTEST_FOUND TRUE)
else()
    message(WARNING "Google Test submodule not found at external/googletest, falling back to FetchContent")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    set(GTEST_FOUND TRUE)
endif()

#==============================================================================
# Test Sources
#==============================================================================

set(TEST_SOURCES
    main.cpp
    event_test.cpp
    event_queue_test.cpp
    event_bus_test.cpp
    input_state_test.cpp
)

#==============================================================================
# Test Executable
#==============================================================================

add_executable(TestVneEvents ${TEST_SOURCES})

target_link_libraries(TestVneEvents
    PRIVATE
        vneevents
        GTest::gtest_main
)
# Test main.cpp uses vnelogging; ensure include path and library are available when present
if(TARGET vne::logging)
    target_link_libraries(TestVneEvents PRIVATE vne::logging)
endif()

# Link against gcov when coverage is enabled
if(ENABLE_COVERAGE AND UNIX AND NOT APPLE)
    target_link_libraries(TestVneEvents PRIVATE gcov)
endif()

target_include_directories(TestVneEvents
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

#==============================================================================
# Register Tests with CTest
#==============================================================================

include(GoogleTest)
gtest_discover_tests(TestVneEvents)

#==============================================================================
# Output Directory
#==============================================================================

set_target_properties(TestVneEvents PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
