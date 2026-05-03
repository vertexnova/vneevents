#==============================================================================
# Copyright (c) 2024 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# Author:    Ajeet Singh Yadav
# Created:   January 2025
#
# Autodoc:   yes
#==============================================================================

cmake_minimum_required(VERSION 3.16)

project(TestVneEvents)

# Disable tests for iOS and Web platforms
if(VNE_PLATFORM_IOS OR VNE_PLATFORM_WEB)
    message(STATUS "Events tests disabled for ${CMAKE_SYSTEM_NAME} platform")
    return()
endif()

#==============================================================================
# Setup Google Test
#==============================================================================

# Use external googletest submodule (v1.17.0)
if(EXISTS "${CMAKE_SOURCE_DIR}/deps/external/googletest/CMakeLists.txt")
    message(STATUS "Using Google Test from deps/external/googletest (v1.17.0)")
    add_subdirectory(${CMAKE_SOURCE_DIR}/deps/external/googletest ${CMAKE_BINARY_DIR}/deps/external/googletest)
    include(GoogleTest)
    set(GTEST_LIBRARIES gtest_main gmock)
    set(GTEST_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/deps/external/googletest/googletest/include
        ${CMAKE_SOURCE_DIR}/deps/external/googletest/googlemock/include
    )
else()
    # Try to find GoogleTest in system
    find_package(GTest QUIET)
    if(GTest_FOUND)
        message(STATUS "Using system Google Test")
        set(GTEST_LIBRARIES GTest::gtest_main GTest::gmock)
        set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})
    else()
        # Fallback to FetchContent
        message(STATUS "deps/external/googletest not found, using FetchContent")
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.17.0
        )
        FetchContent_MakeAvailable(googletest)
        set(GTEST_LIBRARIES gtest_main gmock)
        set(GTEST_INCLUDE_DIRS ${googletest_SOURCE_DIR}/googletest/include ${googletest_SOURCE_DIR}/googlemock/include)
    endif()
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
#                              Build Executable                                #
#==============================================================================

# Create the test executable
add_executable(TestVneEvents ${TEST_SOURCES})

# Include directories
target_include_directories(TestVneEvents
    SYSTEM PUBLIC
        ${GTEST_INCLUDE_DIRS}
    PUBLIC
        $<BUILD_INTERFACE:${VNE_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${VNE_SRC_DIR}>
)

# Link libraries
target_link_libraries(TestVneEvents
    PRIVATE
        ${GTEST_LIBRARIES}
        vne::events
)

if(VNEEVENTS_LIB_TYPE STREQUAL "shared")
    if(WIN32)
        add_custom_command(TARGET TestVneEvents POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:vneevents>
                $<TARGET_FILE_DIR:TestVneEvents>
            VERBATIM)
    else()
        set_target_properties(TestVneEvents PROPERTIES
            BUILD_RPATH "$<TARGET_FILE_DIR:vneevents>"
        )
    endif()
endif()

# Web platform specific configuration
if(VNE_PLATFORM_WEB)
    target_compile_definitions(TestVneEvents PRIVATE VNE_PLATFORM_WEB)
    set_target_properties(TestVneEvents PROPERTIES
        SUFFIX ".js"
    )
endif()

# Add a test
add_test(NAME vneevents.test COMMAND TestVneEvents)
