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
# VneUseDep.cmake — add an internal VertexNova dependency only when needed
#
# Problem this solves
# -------------------
# VneEvents embeds submodules (vnecommon, vnelogging, …). A parent project may
# already provide the same target (e.g. vne::common). Calling add_subdirectory
# twice would fail. vne_use_dep() checks first and no-ops when the target exists.
#
# Typical call (from CMakeLists.txt)
# ----------------------------------
#   list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#   include(VneUseDep)
#
#   vne_use_dep(
#       TARGET vne::common
#       SUBDIR "${VNE_LIBS_DIR}/vnecommon"
#       BINARY_DIR "${CMAKE_BINARY_DIR}/deps/internal/vnecommon"
#       CACHE_VARS VNE_COMMON_SKIP_INSTALL ON
#   )
#
# Arguments
# ---------
#   TARGET     (required) Imported target name to look for, e.g. vne::common
#   SUBDIR     (required) Path to the dependency's CMake project root
#   BINARY_DIR (optional) Out-of-tree build directory for add_subdirectory
#   CACHE_VARS (optional) Flat list of CACHE variable pairs: VAR VALUE VAR VALUE …
#                         Values must be CMake booleans (ON/OFF, TRUE/FALSE, …).
#
# Behavior
# --------
#   1. TARGET already exists  → log "using from parent", return
#   2. SUBDIR has no CMakeLists → warning, return
#   3. Otherwise                → apply CACHE_VARS, then add_subdirectory
#==============================================================================

if(DEFINED VNE_USE_DEP_INCLUDED)
    return()
endif()
set(VNE_USE_DEP_INCLUDED TRUE)

#------------------------------------------------------------------------------
# _vne_use_dep_apply_cache_vars — set CACHE BOOL entries from VAR VALUE pairs
#------------------------------------------------------------------------------
function(_vne_use_dep_apply_cache_vars cache_var_list)
    list(LENGTH ${cache_var_list} _len)
    math(EXPR _mod "${_len} % 2")
    if(NOT _mod EQUAL 0)
        message(FATAL_ERROR
            "vne_use_dep: CACHE_VARS expects an even number of arguments "
            "(VAR VALUE pairs), but got ${_len}")
    endif()

    set(_valid_bools TRUE FALSE ON OFF YES NO Y N 1 0)
    set(_i 0)
    while(_i LESS _len)
        list(GET ${cache_var_list} ${_i} _var)
        math(EXPR _vi "${_i} + 1")
        list(GET ${cache_var_list} ${_vi} _val)

        string(TOUPPER "${_val}" _val_upper)
        list(FIND _valid_bools "${_val_upper}" _bool_idx)
        if(_bool_idx EQUAL -1)
            message(FATAL_ERROR
                "vne_use_dep: CACHE_VARS entry '${_var}' must be a boolean "
                "(TRUE, FALSE, ON, OFF, YES, NO, Y, N, 1, 0); got '${_val}'")
        endif()

        set(${_var} ${_val} CACHE BOOL "" FORCE)
        math(EXPR _i "${_i} + 2")
    endwhile()
endfunction()

#------------------------------------------------------------------------------
# vne_use_dep — public entry point
#------------------------------------------------------------------------------
function(vne_use_dep)
    set(oneValueArgs TARGET SUBDIR BINARY_DIR)
    set(multiValueArgs CACHE_VARS)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_TARGET)
        message(FATAL_ERROR "vne_use_dep: TARGET is required")
    endif()
    if(NOT ARG_SUBDIR)
        message(FATAL_ERROR "vne_use_dep: SUBDIR is required")
    endif()

    # Step 1: parent already provided this dependency
    if(TARGET ${ARG_TARGET})
        message(STATUS "VneDep: using ${ARG_TARGET} from parent")
        return()
    endif()

    # Step 2: submodule not present (optional dependency)
    if(NOT EXISTS "${ARG_SUBDIR}/CMakeLists.txt")
        message(WARNING "VneDep: ${ARG_TARGET} not found at ${ARG_SUBDIR}")
        return()
    endif()

    # Step 3: embed the dependency
    message(STATUS "VneDep: adding ${ARG_TARGET} from ${ARG_SUBDIR}")

    if(ARG_CACHE_VARS)
        _vne_use_dep_apply_cache_vars(ARG_CACHE_VARS)
    endif()

    if(ARG_BINARY_DIR)
        add_subdirectory(${ARG_SUBDIR} ${ARG_BINARY_DIR})
    else()
        add_subdirectory(${ARG_SUBDIR})
    endif()
endfunction()
