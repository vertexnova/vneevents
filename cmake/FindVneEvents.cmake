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
#==============================================================================

include(FindPackageHandleStandardArgs)

find_path(VneEvents_INCLUDE_DIR
    NAMES vertexnova/events/events.h
    PATHS
        ${CMAKE_CURRENT_LIST_DIR}/../../include
        ${CMAKE_INSTALL_PREFIX}/include
        /usr/local/include
)

find_library(VneEvents_LIBRARY
    NAMES VneEvents vneevents
    PATHS
        ${CMAKE_CURRENT_LIST_DIR}/../../lib
        ${CMAKE_INSTALL_PREFIX}/lib
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
