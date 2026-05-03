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
#   VneEvents_RUNTIME_LIBRARY (Windows shared: path to the DLL, when known)
#
# Usage:
#   find_package(VneEvents REQUIRED)
#   target_link_libraries(app PRIVATE vne::events)
#
# Optional (set before find_package, or -D / CACHE):
#   VneEvents_RUNTIME_LIBRARY — full path to vneevents.dll when layout detection fails.
#   VneEvents_IS_SHARED       — TRUE/ON or FALSE/OFF to force shared vs static (skips guessing).
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

# Optional DLL path hint (cache survives reconfigure when set with -D or cmake-gui).
set(VneEvents_RUNTIME_LIBRARY "${VneEvents_RUNTIME_LIBRARY}" CACHE FILEPATH
    "Optional (Windows): full path to vneevents.dll if FindVneEvents cannot find it."
)

set(_VneEvents_result_shared FALSE)
set(_VneEvents_runtime_dll "")

if(VneEvents_LIBRARY)
    if(WIN32)
        get_filename_component(_vne_ev_lib_dir "${VneEvents_LIBRARY}" DIRECTORY)
        get_filename_component(_vne_ev_lib_name "${VneEvents_LIBRARY}" NAME_WE)

        set(_vne_ev_dll "")
        set(_vne_ev_force_shared FALSE)
        set(_vne_ev_force_static FALSE)

        if(VneEvents_RUNTIME_LIBRARY AND NOT VneEvents_RUNTIME_LIBRARY STREQUAL "")
            if(EXISTS "${VneEvents_RUNTIME_LIBRARY}")
                set(_vne_ev_dll "${VneEvents_RUNTIME_LIBRARY}")
            else()
                message(WARNING
                    "FindVneEvents: VneEvents_RUNTIME_LIBRARY is set but file does not exist: "
                    "'${VneEvents_RUNTIME_LIBRARY}'. Ignoring.")
            endif()
        endif()

        if(NOT VneEvents_IS_SHARED STREQUAL "")
            string(TOUPPER "${VneEvents_IS_SHARED}" _vne_ev_is_shared_upper)
            if(_vne_ev_is_shared_upper MATCHES "^(0|OFF|FALSE|NO|N)$")
                set(_vne_ev_force_static TRUE)
            elseif(VneEvents_IS_SHARED)
                set(_vne_ev_force_shared TRUE)
            endif()
        endif()

        if(_vne_ev_force_static)
            set(_VneEvents_result_shared FALSE)
        elseif(_vne_ev_dll)
            set(_VneEvents_result_shared TRUE)
            set(_VneEvents_runtime_dll "${_vne_ev_dll}")
        else()
            # Auto-detect: import libraries usually pair with a DLL elsewhere.
            set(_vne_ev_search_paths
                "${_vne_ev_lib_dir}"
                "${_vne_ev_lib_dir}/../bin"
                "${_vne_ev_lib_dir}/../../bin"
                "${_vne_ev_lib_dir}/../sbin"
                "${_vne_ev_lib_dir}/bin"
            )
            foreach(_vne_pfx IN LISTS CMAKE_PREFIX_PATH)
                if(NOT _vne_pfx STREQUAL "")
                    list(APPEND _vne_ev_search_paths "${_vne_pfx}/bin")
                endif()
            endforeach()
            if(CMAKE_INSTALL_PREFIX)
                list(APPEND _vne_ev_search_paths "${CMAKE_INSTALL_PREFIX}/bin")
            endif()

            foreach(_vne_ddir IN LISTS _vne_ev_search_paths)
                if(EXISTS "${_vne_ddir}/${_vne_ev_lib_name}.dll")
                    set(_vne_ev_dll "${_vne_ddir}/${_vne_ev_lib_name}.dll")
                    break()
                endif()
            endforeach()

            if(NOT _vne_ev_dll)
                find_file(_vne_ev_dll_env "${_vne_ev_lib_name}.dll"
                    PATHS ENV PATH
                    DOC "FindVneEvents: search PATH for runtime DLL"
                )
                if(_vne_ev_dll_env AND EXISTS "${_vne_ev_dll_env}")
                    set(_vne_ev_dll "${_vne_ev_dll_env}")
                endif()
            endif()

            if(_vne_ev_force_shared AND NOT _vne_ev_dll)
                message(WARNING
                    "FindVneEvents: VneEvents_IS_SHARED is ON but '${_vne_ev_lib_name}.dll' was not found "
                    "(searched next to the import library, common bin/ layouts, CMAKE_PREFIX_PATH/bin, "
                    "CMAKE_INSTALL_PREFIX/bin, and PATH). Set VneEvents_RUNTIME_LIBRARY to the DLL path. "
                    "Linking as shared without IMPORTED_LOCATION may be incorrect; "
                    "consumers linking the DLL must define VNEEVENTS_DLL.")
                set(_VneEvents_result_shared TRUE)
            elseif(_vne_ev_dll)
                set(_VneEvents_result_shared TRUE)
                set(_VneEvents_runtime_dll "${_vne_ev_dll}")
            else()
                message(WARNING
                    "FindVneEvents: could not find '${_vne_ev_lib_name}.dll' for import library "
                    "'${VneEvents_LIBRARY}'. Treating as static for the imported target. "
                    "If this is a shared build, set VneEvents_RUNTIME_LIBRARY to the DLL path "
                    "or place the DLL under a searched bin directory and use VneEvents_IS_SHARED=ON. "
                    "Applications linking the shared vneevents DLL must define VNEEVENTS_DLL.")
            endif()
        endif()
    elseif(APPLE)
        if(VneEvents_LIBRARY MATCHES "\\.dylib$")
            set(_VneEvents_result_shared TRUE)
        endif()
    else()
        if(VneEvents_LIBRARY MATCHES "\\.so(\\.[0-9]+)*$")
            set(_VneEvents_result_shared TRUE)
        endif()
    endif()
endif()

set(VneEvents_IS_SHARED ${_VneEvents_result_shared})
if(_VneEvents_runtime_dll)
    set(VneEvents_RUNTIME_LIBRARY "${_VneEvents_runtime_dll}")
endif()
unset(_VneEvents_result_shared)
unset(_VneEvents_runtime_dll)

find_package_handle_standard_args(VneEvents
    FOUND_VAR VneEvents_FOUND
    REQUIRED_VARS VneEvents_LIBRARY VneEvents_INCLUDE_DIR
)

if(VneEvents_FOUND)
    if(NOT TARGET vne::events)
        if(VneEvents_IS_SHARED)
            add_library(vne::events SHARED IMPORTED)
            if(WIN32 AND DEFINED VneEvents_RUNTIME_LIBRARY AND NOT VneEvents_RUNTIME_LIBRARY STREQUAL "")
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

mark_as_advanced(VneEvents_INCLUDE_DIR VneEvents_LIBRARY VneEvents_RUNTIME_LIBRARY)

unset(_VneEvents_search_prefix)
unset(_VneEvents_probe)
unset(_VneEvents_parent)
unset(_VneEvents_include_paths)
unset(_VneEvents_library_paths)
