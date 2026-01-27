#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# Author:    Ajeet Singh Yadav
# Created:   January 2026
#
# VertexNova Events - Lightweight, thread-safe event system
#==============================================================================

# Minimum required CMake version
cmake_minimum_required(VERSION 3.16)

# Project declaration
project(vnemath VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

#==============================================================================
# Add custom CMake modules path
#==============================================================================
if(EXISTS "${PROJECT_SOURCE_DIR}/cmake/vnecmake/modules")
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/vnecmake/modules")
endif()
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#==============================================================================
# Internal Libraries
#==============================================================================
set(VNE_LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)

if(NOT TARGET vne::common)
    if(EXISTS "${VNE_LIBS_DIR}/vnecommon/CMakeLists.txt")
        # vnecommon is header-only; we don't need its example executable
        set(VNEEVENTS_BUILD_EXAMPLES ${BUILD_EXAMPLES})
        set(BUILD_EXAMPLES OFF)
        add_subdirectory(libs/vnecommon)
        set(BUILD_EXAMPLES ${VNEEVENTS_BUILD_EXAMPLES})
    endif()
endif()

# VneLogging - v1.0.0
if(NOT TARGET vne::logging)
    if(EXISTS "${VNE_LIBS_DIR}/vnelogging/CMakeLists.txt")
        message(STATUS "Using VneLogging from libs/vnelogging (v1.0.0)")
        # Save current BUILD_TESTS and BUILD_EXAMPLES values
        set(VNEEVENTS_BUILD_TESTS ${BUILD_TESTS})
        set(VNEEVENTS_BUILD_EXAMPLES ${BUILD_EXAMPLES})
        # Disable vnelogging tests and examples to avoid conflicts
        set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${VNE_LIBS_DIR}/vnelogging ${CMAKE_BINARY_DIR}/libs/vnelogging)
        # Restore BUILD_TESTS and BUILD_EXAMPLES for vneevents
        set(BUILD_TESTS ${VNEEVENTS_BUILD_TESTS} CACHE BOOL "Build the test suite" FORCE)
        set(BUILD_EXAMPLES ${VNEEVENTS_BUILD_EXAMPLES} CACHE BOOL "Build example programs" FORCE)
    else()
        message(WARNING "VneLogging submodule not found at libs/vnelogging")
    endif()
endif()

#==============================================================================
# Compile Commands (for clang-tidy, IDE, etc.)
#==============================================================================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#==============================================================================
# Build Options
#==============================================================================

option(BUILD_TESTS "Build the test suite" ON)
option(BUILD_EXAMPLES "Build example programs" OFF)
option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)

#==============================================================================
# Coverage Support
#==============================================================================
include(FindCoverage)

#==============================================================================
# Documentation Support
#==============================================================================
option(ENABLE_DOXYGEN "Enable Doxygen documentation builds" OFF)
if(ENABLE_DOXYGEN)
    include(Doxygen)
    enable_doxygen()
endif()

#==============================================================================
# Platform Detection
#==============================================================================

if(NOT DEFINED VNE_TARGET_PLATFORM)
    if(EMSCRIPTEN)
        set(VNE_TARGET_PLATFORM "Web")
    elseif(WIN32)
        set(VNE_TARGET_PLATFORM "Windows")
    elseif(APPLE)
        if(IOS OR ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
            set(VNE_TARGET_PLATFORM "iOS")
        else()
            set(VNE_TARGET_PLATFORM "macOS")
        endif()
    elseif(UNIX)
        if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
            set(VNE_TARGET_PLATFORM "Android")
        else()
            set(VNE_TARGET_PLATFORM "Linux")
        endif()
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()
endif()

message(STATUS "VneEvents: Detected platform: ${VNE_TARGET_PLATFORM}")

#==============================================================================
# Compiler Warnings
#==============================================================================

add_library(VneEventsWarnings INTERFACE)
target_compile_options(VneEventsWarnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
add_library(vne::events::Warnings ALIAS VneEventsWarnings)

#==============================================================================
# Build Settings
#==============================================================================

add_library(VneEventsBuildSettings INTERFACE)
add_library(vne::events::BuildSettings ALIAS VneEventsBuildSettings)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(VneEventsBuildSettings INTERFACE VNE_BUILD_TYPE_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(VneEventsBuildSettings INTERFACE VNE_BUILD_TYPE_RELEASE)
endif()

#==============================================================================
# Include Directories
#==============================================================================

set(VNE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(VNE_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

#==============================================================================
# Source Files
#==============================================================================

set(VNEEVENTS_SOURCES
    ${VNE_SRC_DIR}/vertexnova/events/input/input_state.cpp
    ${VNE_SRC_DIR}/vertexnova/events/input/input_manager.cpp
)

set(VNEEVENTS_HEADERS
    ${VNE_INCLUDE_DIR}/vertexnova/events/events.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/types.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/event.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/event_listener.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/event_queue.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/event_dispatcher.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/event_bus.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/event_manager.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/key_event.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/mouse_event.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/window_event.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/touch_event.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/input/input.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/input/input_manager.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/input/input_state.h
    ${VNE_INCLUDE_DIR}/vertexnova/events/internal/read_write_mutex.h
)

#==============================================================================
# Library Target
#==============================================================================

add_library(vneevents ${VNEEVENTS_SOURCES} ${VNEEVENTS_HEADERS})
add_library(vne::events ALIAS vneevents)

target_include_directories(vneevents
    PUBLIC
        $<BUILD_INTERFACE:${VNE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(vneevents
    PUBLIC
        VneEventsWarnings
        VneEventsBuildSettings
)

if(TARGET vne::common)
    target_link_libraries(vneevents PUBLIC vne::common)
endif()

# Link against VneLogging if available (optional, for console logging)
if(TARGET vne::logging)
    target_link_libraries(vneevents PUBLIC vne::logging)
    message(STATUS "VneEvents: Linked against vne::logging")
elseif(TARGET vnelogging)
    target_link_libraries(vneevents PUBLIC vnelogging)
    message(STATUS "VneEvents: Linked against vnelogging")
endif()

set_target_properties(vneevents PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME vneevents
)

#==============================================================================
# Tests
#==============================================================================

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

#==============================================================================
# Examples
#==============================================================================

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

#==============================================================================
# Installation
#==============================================================================

include(GNUInstallDirs)

install(TARGETS vneevents VneEventsWarnings VneEventsBuildSettings
    EXPORT VneEventsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/vertexnova/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vertexnova
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT VneEventsTargets
    FILE VneEventsTargets.cmake
    NAMESPACE vne::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VneEvents
)
