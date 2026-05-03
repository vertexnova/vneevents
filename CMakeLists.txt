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
project(vneevents VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#==============================================================================
# Library type (must be set before add_subdirectory(vnelogging))
#==============================================================================
set(VNEEVENTS_LIB_TYPE "static" CACHE STRING "VneEvents library type: static or shared (one build directory)")
set_property(CACHE VNEEVENTS_LIB_TYPE PROPERTY STRINGS static shared)
if(NOT VNEEVENTS_LIB_TYPE STREQUAL "static" AND NOT VNEEVENTS_LIB_TYPE STREQUAL "shared")
    message(FATAL_ERROR "VNEEVENTS_LIB_TYPE must be 'static' or 'shared'. Got: ${VNEEVENTS_LIB_TYPE}")
endif()

if(VNEEVENTS_LIB_TYPE STREQUAL "shared")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(_VNEEVENTS_LIB_KIND SHARED)
else()
    set(_VNEEVENTS_LIB_KIND STATIC)
endif()

message(STATUS "VneEvents: VNEEVENTS_LIB_TYPE=${VNEEVENTS_LIB_TYPE}")

function(vneevents_apply_shared_export target_name)
    get_target_property(_vne_ev_kind "${target_name}" TYPE)
    if(NOT _vne_ev_kind STREQUAL "SHARED_LIBRARY")
        return()
    endif()
    target_compile_definitions(${target_name}
        PRIVATE VNEEVENTS_BUILDING_DLL
        INTERFACE VNEEVENTS_DLL
    )
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /wd4251 /wd4275)
    else()
        set_target_properties(${target_name} PROPERTIES
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON
        )
    endif()
endfunction()

#==============================================================================
# Add vnecmake submodule modules directory if present (preserve prior behavior
# from deps/external/vnecmake/modules now that submodule lives at cmake/vnecmake)
#==============================================================================
if(EXISTS "${PROJECT_SOURCE_DIR}/cmake/vnecmake/modules")
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/vnecmake/modules")
else()
    message(WARNING "vnecmake submodule not found at cmake/vnecmake; run git submodule update --init")
endif()

#==============================================================================
# Internal Libraries
#==============================================================================
set(VNE_LIBS_DIR ${PROJECT_SOURCE_DIR}/deps/internal)

if(NOT TARGET vne::common)
    if(EXISTS "${VNE_LIBS_DIR}/vnecommon/CMakeLists.txt")
        # vnecommon is header-only; we don't need its example executable
        set(VNEEVENTS_BUILD_EXAMPLES ${BUILD_EXAMPLES})
        set(BUILD_EXAMPLES OFF)
        add_subdirectory(deps/internal/vnecommon)
        set(BUILD_EXAMPLES ${VNEEVENTS_BUILD_EXAMPLES})
    endif()
endif()

# VneLogging - v1.0.0 (provide VneUseDep from vneevents so vnelogging submodule stays clean)
if(NOT TARGET vne::logging)
    if(EXISTS "${VNE_LIBS_DIR}/vnelogging/CMakeLists.txt")
        set(VNE_LOGGING_LIB_TYPE "${VNEEVENTS_LIB_TYPE}" CACHE STRING "" FORCE)
        message(STATUS "Using VneLogging from deps/internal/vnelogging (v1.0.0)")
        # Save current BUILD_TESTS and BUILD_EXAMPLES values
        set(VNEEVENTS_BUILD_TESTS ${BUILD_TESTS})
        set(VNEEVENTS_BUILD_EXAMPLES ${BUILD_EXAMPLES})
        # Disable vnelogging tests and examples to avoid conflicts
        set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${VNE_LIBS_DIR}/vnelogging ${CMAKE_BINARY_DIR}/deps/internal/vnelogging)
        # Restore BUILD_TESTS and BUILD_EXAMPLES for vneevents
        set(BUILD_TESTS ${VNEEVENTS_BUILD_TESTS} CACHE BOOL "Build the test suite" FORCE)
        set(BUILD_EXAMPLES ${VNEEVENTS_BUILD_EXAMPLES} CACHE BOOL "Build example programs" FORCE)
    else()
        message(WARNING "VneLogging submodule not found at deps/internal/vnelogging")
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
    ${VNE_SRC_DIR}/vertexnova/events/input/input.cpp
    ${VNE_SRC_DIR}/vertexnova/events/input/input_state.cpp
    ${VNE_SRC_DIR}/vertexnova/events/input/input_manager.cpp
)

set(VNEEVENTS_HEADERS
    ${VNE_INCLUDE_DIR}/vertexnova/events/export.h
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

add_library(vneevents ${_VNEEVENTS_LIB_KIND} ${VNEEVENTS_SOURCES} ${VNEEVENTS_HEADERS})
add_library(vne::events ALIAS vneevents)

vneevents_apply_shared_export(vneevents)

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
if(NOT CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    if(DEFINED ENV{CI} AND DEFINED ENV{GITHUB_WORKSPACE})
        file(TO_CMAKE_PATH "$ENV{GITHUB_WORKSPACE}" _VNE_GITHUB_WORKSPACE)
        set(_VNE_IOS_CI_INSTALL_PREFIX "${_VNE_GITHUB_WORKSPACE}/install")
        if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
                OR CMAKE_INSTALL_PREFIX STREQUAL "${CMAKE_BINARY_DIR}/install")
            set(CMAKE_INSTALL_PREFIX "${_VNE_IOS_CI_INSTALL_PREFIX}"
                CACHE PATH "Install prefix for iOS on CI (workspace/install)" FORCE)
        endif()
    elseif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install"
            CACHE PATH "Install prefix for iOS builds (override with -DCMAKE_INSTALL_PREFIX)" FORCE)
    endif()
endif()

install(TARGETS vneevents VneEventsWarnings VneEventsBuildSettings
    EXPORT VneEventsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/vertexnova/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vertexnova
    FILES_MATCHING PATTERN "*.h"
)

set(VNECOMMON_PUBLIC_HEADERS_DIR
    "${PROJECT_SOURCE_DIR}/deps/internal/vnecommon/include/vertexnova/common")
if(NOT EXISTS "${VNECOMMON_PUBLIC_HEADERS_DIR}")
    message(FATAL_ERROR
        "Required public dependency headers were not found: "
        "${VNECOMMON_PUBLIC_HEADERS_DIR}. "
        "Ensure deps/internal/vnecommon is available (git submodule update --init).")
endif()
install(DIRECTORY "${VNECOMMON_PUBLIC_HEADERS_DIR}/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vertexnova/common
    FILES_MATCHING PATTERN "*.h"
)

if(EXISTS "${PROJECT_SOURCE_DIR}/deps/internal/vnelogging/include/vertexnova/logging")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/deps/internal/vnelogging/include/vertexnova/logging/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vertexnova/logging
        FILES_MATCHING PATTERN "*.h"
    )
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/deps/internal/vnelogging/src/vertexnova/logging")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/deps/internal/vnelogging/src/vertexnova/logging/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vertexnova/logging
        FILES_MATCHING PATTERN "*.h"
    )
endif()

install(EXPORT VneEventsTargets
    FILE VneEventsTargets.cmake
    NAMESPACE vne::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VneEvents
)

install(FILES "${PROJECT_SOURCE_DIR}/cmake/FindVneEvents.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VneEvents
)

if(EXISTS "${PROJECT_SOURCE_DIR}/LICENSE")
    install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION .)
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/CHANGELOG.md")
    install(FILES "${PROJECT_SOURCE_DIR}/CHANGELOG.md" DESTINATION .)
endif()
