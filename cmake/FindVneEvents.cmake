#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
#
# FindVneEvents.cmake — fallback finder when CMake package config is absent.
#
# Defines:
#   VneEvents_FOUND
#   VneEvents_INCLUDE_DIRS
#   VneEvents_LIBRARIES
#   VneEvents_IS_SHARED
#
# Usage:
#   find_package(VneEvents REQUIRED)
#   target_link_libraries(app PRIVATE vne::events)
#
# This module may live at:
#   - <prefix>/<libdir>/cmake/VneEvents/FindVneEvents.cmake (installed)
#   - <repo>/cmake/FindVneEvents.cmake (source checkout on CMAKE_MODULE_PATH)
#
# Search hints must not assume a fixed depth (../../include breaks once installed:
# that resolves to <prefix>/lib/include). We walk upward until we find the public
# header tree at include/vertexnova/events/events.h, then use that prefix for lib/.
#==============================================================================

include(FindPackageHandleStandardArgs)

# Discover install / checkout prefix by walking parents of this file's directory.
set(_VneEvents_search_prefix "")
set(_VneEvents_probe "${CMAKE_CURRENT_LIST_DIR}")
foreach(_vne_i RANGE 1 12)
    if(EXISTS "${_VneEvents_probe}/include/vertexnova/events/events.h")
        get_filename_component(_VneEvents_search_prefix "${_VneEvents_probe}" ABSOLUTE)
        break()
    endif()
    get_filename_component(_VneEvents_parent "${_VneEvents_probe}" DIRECTORY)
    if(_VneEvents_parent STREQUAL _VneEvents_probe OR _VneEvents_parent STREQUAL "")
        break()
    endif()
    set(_VneEvents_probe "${_VneEvents_parent}")
endforeach()

set(_VneEvents_include_paths "")
set(_VneEvents_library_paths "")
if(_VneEvents_search_prefix)
    list(APPEND _VneEvents_include_paths "${_VneEvents_search_prefix}/include")
    list(APPEND _VneEvents_library_paths
        "${_VneEvents_search_prefix}/lib"
        "${_VneEvents_search_prefix}/lib64"
    )
    if(CMAKE_LIBRARY_ARCHITECTURE)
        list(APPEND _VneEvents_library_paths
            "${_VneEvents_search_prefix}/lib/${CMAKE_LIBRARY_ARCHITECTURE}"
        )
    endif()
endif()

find_path(VneEvents_INCLUDE_DIR
    NAMES vertexnova/events/events.h
    PATHS
        ${_VneEvents_include_paths}
        ${CMAKE_INSTALL_PREFIX}/include
        /usr/local/include
)

find_library(VneEvents_LIBRARY
    NAMES VneEvents vneevents
    PATHS
        ${_VneEvents_library_paths}
        ${CMAKE_INSTALL_PREFIX}/lib
        ${CMAKE_INSTALL_PREFIX}/lib64
        /usr/local/lib
)

set(VneEvents_IS_SHARED FALSE)
if(VneEvents_LIBRARY)
    if(WIN32)
        get_filename_component(_vne_ev_lib_dir "${VneEvents_LIBRARY}" DIRECTORY)
        get_filename_component(_vne_ev_lib_name "${VneEvents_LIBRARY}" NAME_WE)
        if(EXISTS "${_vne_ev_lib_dir}/${_vne_ev_lib_name}.dll")
            set(VneEvents_IS_SHARED TRUE)
            set(VneEvents_RUNTIME_LIBRARY "${_vne_ev_lib_dir}/${_vne_ev_lib_name}.dll")
        elseif(EXISTS "${_vne_ev_lib_dir}/../bin/${_vne_ev_lib_name}.dll")
            set(VneEvents_IS_SHARED TRUE)
            set(VneEvents_RUNTIME_LIBRARY "${_vne_ev_lib_dir}/../bin/${_vne_ev_lib_name}.dll")
        endif()
    elseif(APPLE)
        if(VneEvents_LIBRARY MATCHES "\\.dylib$")
            set(VneEvents_IS_SHARED TRUE)
        endif()
    else()
        if(VneEvents_LIBRARY MATCHES "\\.so(\\.[0-9]+)*$")
            set(VneEvents_IS_SHARED TRUE)
        endif()
    endif()
endif()

find_package_handle_standard_args(VneEvents
    FOUND_VAR VneEvents_FOUND
    REQUIRED_VARS VneEvents_LIBRARY VneEvents_INCLUDE_DIR
)

if(VneEvents_FOUND)
    if(NOT TARGET vne::events)
        if(VneEvents_IS_SHARED)
            add_library(vne::events SHARED IMPORTED)
            if(WIN32 AND DEFINED VneEvents_RUNTIME_LIBRARY)
                set_target_properties(vne::events PROPERTIES
                    IMPORTED_IMPLIB "${VneEvents_LIBRARY}"
                    IMPORTED_LOCATION "${VneEvents_RUNTIME_LIBRARY}"
                    INTERFACE_INCLUDE_DIRECTORIES "${VneEvents_INCLUDE_DIR}"
                    INTERFACE_COMPILE_DEFINITIONS "VNEEVENTS_DLL"
                )
            else()
                set_target_properties(vne::events PROPERTIES
                    IMPORTED_LOCATION "${VneEvents_LIBRARY}"
                    INTERFACE_INCLUDE_DIRECTORIES "${VneEvents_INCLUDE_DIR}"
                    INTERFACE_COMPILE_DEFINITIONS "VNEEVENTS_DLL"
                )
            endif()
        else()
            add_library(vne::events STATIC IMPORTED)
            set_target_properties(vne::events PROPERTIES
                IMPORTED_LOCATION "${VneEvents_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${VneEvents_INCLUDE_DIR}"
            )
        endif()
    endif()
    set(VneEvents_LIBRARIES ${VneEvents_LIBRARY})
    set(VneEvents_INCLUDE_DIRS ${VneEvents_INCLUDE_DIR})
endif()

mark_as_advanced(VneEvents_INCLUDE_DIR VneEvents_LIBRARY)

unset(_VneEvents_search_prefix)
unset(_VneEvents_probe)
unset(_VneEvents_parent)
unset(_VneEvents_include_paths)
unset(_VneEvents_library_paths)
