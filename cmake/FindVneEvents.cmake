#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# Author:    Ajeet Singh Yadav
# Created:   January 2026
#
# Autodoc:   yes
#==============================================================================
#
# FindVneEvents.cmake — MODULE-mode finder (fallback when CONFIG package is absent)
#
# Quick start
# -----------
#   find_package(VneEvents REQUIRED)
#   target_link_libraries(my_app PRIVATE vne::events)
#
# Prefer CONFIG mode when you have an install tree:
#   find_package(VneEvents CONFIG REQUIRED)   # uses VneEventsConfig.cmake
#
# This Find module runs when CONFIG is not used and this file is on CMAKE_MODULE_PATH.
#
# Variables set on success
# ------------------------
#   VneEvents_FOUND
#   VneEvents_INCLUDE_DIRS
#   VneEvents_LIBRARIES
#   VneEvents_IS_SHARED        TRUE when a shared/DLL artifact was detected
#   VneEvents_RUNTIME_LIBRARY  Windows shared: path to vneevents.dll (when known)
#
# Optional hints (set before find_package, or via -D / CACHE)
# -----------------------------------------------------------
#   VneEvents_RUNTIME_LIBRARY  Full path to vneevents.dll when auto-detection fails
#   VneEvents_IS_SHARED        Force TRUE/FALSE to skip shared-vs-static guessing
#
# Where this file lives
# ---------------------
#   Installed:  <prefix>/lib/cmake/VneEvents/FindVneEvents.cmake
#   Source:     <repo>/cmake/FindVneEvents.cmake  (on CMAKE_MODULE_PATH)
#
# File layout this module expects
# -------------------------------
#   <prefix>/include/vertexnova/events/events.h
#   <prefix>/lib/libvneevents.{a,so,dylib,...}
#   <prefix>/bin/vneevents.dll                  (Windows shared, typical)
#==============================================================================

include(FindPackageHandleStandardArgs)

#------------------------------------------------------------------------------
# Step 1 helpers — discover install prefix by walking up from this file
#------------------------------------------------------------------------------
#
# We cannot assume a fixed relative depth (e.g. ../../include) because once
# installed this file sits under lib/cmake/VneEvents/, and ../../include would
# wrongly resolve to <prefix>/lib/include. Instead we walk parents until we find
# include/vertexnova/events/events.h and treat that directory as <prefix>.

function(_vneevents_discover_prefix out_prefix)
    set(_probe "${CMAKE_CURRENT_LIST_DIR}")
    set(_found_prefix "")

    foreach(_unused RANGE 1 12)
        if(EXISTS "${_probe}/include/vertexnova/events/events.h")
            get_filename_component(_found_prefix "${_probe}" ABSOLUTE)
            break()
        endif()
        get_filename_component(_parent "${_probe}" DIRECTORY)
        if(_parent STREQUAL _probe OR _parent STREQUAL "")
            break()
        endif()
        set(_probe "${_parent}")
    endforeach()

    set(${out_prefix} "${_found_prefix}" PARENT_SCOPE)
endfunction()

function(_vneevents_search_paths_from_prefix prefix out_includes out_libs)
    set(_includes "")
    set(_libs "")
    if(prefix)
        list(APPEND _includes "${prefix}/include")
        list(APPEND _libs
            "${prefix}/lib"
            "${prefix}/lib64"
        )
        if(CMAKE_LIBRARY_ARCHITECTURE)
            list(APPEND _libs "${prefix}/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
        endif()
    endif()
    set(${out_includes} "${_includes}" PARENT_SCOPE)
    set(${out_libs} "${_libs}" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
# Step 2 helpers — Windows shared vs static (import lib + DLL pairing)
#------------------------------------------------------------------------------

function(_vneevents_windows_classify_library lib_path user_is_shared_hint out_is_shared out_runtime_dll)
    set(_is_shared FALSE)
    set(_runtime_dll "")

    get_filename_component(_lib_dir "${lib_path}" DIRECTORY)
    get_filename_component(_lib_name "${lib_path}" NAME_WE)

    # --- optional user hints ---
    set(_dll_hint "")
    if(VneEvents_RUNTIME_LIBRARY AND NOT VneEvents_RUNTIME_LIBRARY STREQUAL "")
        if(EXISTS "${VneEvents_RUNTIME_LIBRARY}")
            set(_dll_hint "${VneEvents_RUNTIME_LIBRARY}")
        else()
            message(WARNING
                "FindVneEvents: VneEvents_RUNTIME_LIBRARY is set but file does not exist: "
                "'${VneEvents_RUNTIME_LIBRARY}'. Ignoring.")
        endif()
    endif()

    set(_force_shared FALSE)
    set(_force_static FALSE)
    if(NOT user_is_shared_hint STREQUAL "")
        string(TOUPPER "${user_is_shared_hint}" _is_shared_upper)
        if(_is_shared_upper MATCHES "^(0|OFF|FALSE|NO|N)$")
            set(_force_static TRUE)
        elseif(user_is_shared_hint)
            set(_force_shared TRUE)
        endif()
    endif()

    # --- classify ---
    if(_force_static)
        set(_is_shared FALSE)
    elseif(_dll_hint)
        set(_is_shared TRUE)
        set(_runtime_dll "${_dll_hint}")
    else()
        # Auto-detect: import libraries pair with a DLL in bin/ or nearby.
        set(_dll_search_dirs
            "${_lib_dir}"
            "${_lib_dir}/../bin"
            "${_lib_dir}/../../bin"
            "${_lib_dir}/../sbin"
            "${_lib_dir}/bin"
        )
        foreach(_pfx IN LISTS CMAKE_PREFIX_PATH)
            if(NOT _pfx STREQUAL "")
                list(APPEND _dll_search_dirs "${_pfx}/bin")
            endif()
        endforeach()
        if(CMAKE_INSTALL_PREFIX)
            list(APPEND _dll_search_dirs "${CMAKE_INSTALL_PREFIX}/bin")
        endif()

        set(_found_dll "")
        foreach(_dir IN LISTS _dll_search_dirs)
            if(EXISTS "${_dir}/${_lib_name}.dll")
                set(_found_dll "${_dir}/${_lib_name}.dll")
                break()
            endif()
        endforeach()

        if(NOT _found_dll)
            find_file(_dll_on_path "${_lib_name}.dll"
                PATHS ENV PATH
                DOC "FindVneEvents: search PATH for runtime DLL"
            )
            if(_dll_on_path AND EXISTS "${_dll_on_path}")
                set(_found_dll "${_dll_on_path}")
            endif()
        endif()

        if(_force_shared AND NOT _found_dll)
            message(WARNING
                "FindVneEvents: VneEvents_IS_SHARED is ON but '${_lib_name}.dll' was not found "
                "(searched next to the import library, common bin/ layouts, CMAKE_PREFIX_PATH/bin, "
                "CMAKE_INSTALL_PREFIX/bin, and PATH). Set VneEvents_RUNTIME_LIBRARY to the DLL path. "
                "Linking as shared without IMPORTED_LOCATION may be incorrect; "
                "consumers linking the DLL must define VNEEVENTS_DLL.")
            set(_is_shared TRUE)
        elseif(_found_dll)
            set(_is_shared TRUE)
            set(_runtime_dll "${_found_dll}")
        else()
            message(WARNING
                "FindVneEvents: could not find '${_lib_name}.dll' for import library "
                "'${lib_path}'. Treating as static for the imported target. "
                "If this is a shared build, set VneEvents_RUNTIME_LIBRARY to the DLL path "
                "or place the DLL under a searched bin directory and use VneEvents_IS_SHARED=ON. "
                "Applications linking the shared vneevents DLL must define VNEEVENTS_DLL.")
        endif()
    endif()

    set(${out_is_shared} "${_is_shared}" PARENT_SCOPE)
    set(${out_runtime_dll} "${_runtime_dll}" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
# Step 2 helpers — Unix / macOS: extension-based shared detection
#------------------------------------------------------------------------------

function(_vneevents_unix_classify_library lib_path out_is_shared)
    set(_is_shared FALSE)
    if(APPLE)
        if(lib_path MATCHES "\\.dylib$")
            set(_is_shared TRUE)
        endif()
    else()
        if(lib_path MATCHES "\\.so(\\.[0-9]+)*$")
            set(_is_shared TRUE)
        endif()
    endif()
    set(${out_is_shared} "${_is_shared}" PARENT_SCOPE)
endfunction()

function(_vneevents_detect_shared lib_path user_is_shared_hint out_is_shared out_runtime_dll)
    set(_is_shared FALSE)
    set(_runtime_dll "")

    if(WIN32)
        _vneevents_windows_classify_library("${lib_path}" "${user_is_shared_hint}" _is_shared _runtime_dll)
    else()
        _vneevents_unix_classify_library("${lib_path}" _is_shared)
    endif()

    set(${out_is_shared} "${_is_shared}" PARENT_SCOPE)
    set(${out_runtime_dll} "${_runtime_dll}" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
# Step 3 helper — create the vne::events imported target
#------------------------------------------------------------------------------

function(_vneevents_create_imported_target include_dir lib_path is_shared runtime_dll)
    if(TARGET vne::events)
        return()
    endif()

    if(is_shared)
        add_library(vne::events SHARED IMPORTED)
        if(WIN32 AND runtime_dll AND NOT runtime_dll STREQUAL "")
            set_target_properties(vne::events PROPERTIES
                IMPORTED_IMPLIB "${lib_path}"
                IMPORTED_LOCATION "${runtime_dll}"
                INTERFACE_INCLUDE_DIRECTORIES "${include_dir}"
                INTERFACE_COMPILE_DEFINITIONS "VNEEVENTS_DLL"
            )
        else()
            set_target_properties(vne::events PROPERTIES
                IMPORTED_LOCATION "${lib_path}"
                INTERFACE_INCLUDE_DIRECTORIES "${include_dir}"
                INTERFACE_COMPILE_DEFINITIONS "VNEEVENTS_DLL"
            )
        endif()
    else()
        add_library(vne::events STATIC IMPORTED)
        set_target_properties(vne::events PROPERTIES
            IMPORTED_LOCATION "${lib_path}"
            INTERFACE_INCLUDE_DIRECTORIES "${include_dir}"
        )
    endif()
endfunction()

#==============================================================================
# Main find logic (read top-to-bottom)
#==============================================================================

# Step 1: locate headers and library on disk
_vneevents_discover_prefix(_VneEvents_prefix)
_vneevents_search_paths_from_prefix("${_VneEvents_prefix}" _VneEvents_include_paths _VneEvents_library_paths)

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

# Step 2: shared vs static + Windows DLL path
set(VneEvents_RUNTIME_LIBRARY "${VneEvents_RUNTIME_LIBRARY}" CACHE FILEPATH
    "Optional (Windows): full path to vneevents.dll if FindVneEvents cannot find it."
)

# Preserve consumer hint; do not assign VneEvents_IS_SHARED until detection finishes.
set(_VneEvents_user_is_shared_hint "")
if(NOT VneEvents_IS_SHARED STREQUAL "")
    set(_VneEvents_user_is_shared_hint "${VneEvents_IS_SHARED}")
endif()

set(_VneEvents_detected_is_shared FALSE)
set(_VneEvents_runtime_dll "")
if(VneEvents_LIBRARY)
    _vneevents_detect_shared(
        "${VneEvents_LIBRARY}"
        "${_VneEvents_user_is_shared_hint}"
        _VneEvents_detected_is_shared
        _VneEvents_runtime_dll
    )
    if(_VneEvents_runtime_dll)
        set(VneEvents_RUNTIME_LIBRARY "${_VneEvents_runtime_dll}")
    endif()
endif()
set(VneEvents_IS_SHARED ${_VneEvents_detected_is_shared})

# Step 3: standard find_package result + imported target
find_package_handle_standard_args(VneEvents
    FOUND_VAR VneEvents_FOUND
    REQUIRED_VARS VneEvents_LIBRARY VneEvents_INCLUDE_DIR
)

if(VneEvents_FOUND)
    _vneevents_create_imported_target(
        "${VneEvents_INCLUDE_DIR}"
        "${VneEvents_LIBRARY}"
        "${VneEvents_IS_SHARED}"
        "${VneEvents_RUNTIME_LIBRARY}"
    )
    set(VneEvents_LIBRARIES ${VneEvents_LIBRARY})
    set(VneEvents_INCLUDE_DIRS ${VneEvents_INCLUDE_DIR})
endif()

mark_as_advanced(VneEvents_INCLUDE_DIR VneEvents_LIBRARY VneEvents_RUNTIME_LIBRARY)

unset(_VneEvents_prefix)
unset(_VneEvents_include_paths)
unset(_VneEvents_library_paths)
unset(_VneEvents_runtime_dll)
unset(_VneEvents_user_is_shared_hint)
unset(_VneEvents_detected_is_shared)
