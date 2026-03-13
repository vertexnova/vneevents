#==============================================================================
# VneUseDep.cmake - Use a VertexNova dependency if already in build, else add it
#
# Provided by vneevents so that deps/internal/vnelogging can be built without
# requiring vnelogging's own cmake/vnecmake submodule. When vne::common (or
# other targets) are already added by the parent, this module no-ops.
#
# Usage: same as in vnelogging's copy:
#   vne_use_dep(TARGET vne::common SUBDIR "${DIR}/vnecommon" BINARY_DIR "${BIN}/vnecommon")
#==============================================================================

if(DEFINED VNE_USE_DEP_INCLUDED)
    return()
endif()
set(VNE_USE_DEP_INCLUDED TRUE)

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

    # If target already in build (e.g. added by parent), do nothing
    if(TARGET ${ARG_TARGET})
        message(STATUS "VneDep: using ${ARG_TARGET} from parent")
        return()
    endif()

    if(NOT EXISTS "${ARG_SUBDIR}/CMakeLists.txt")
        message(WARNING "VneDep: ${ARG_TARGET} not found at ${ARG_SUBDIR}")
        return()
    endif()

    message(STATUS "VneDep: adding ${ARG_TARGET} from ${ARG_SUBDIR}")

    if(ARG_CACHE_VARS)
        list(LENGTH ARG_CACHE_VARS _len)
        # CACHE_VARS must be a flat list of VAR VALUE pairs
        math(EXPR _mod "${_len} % 2")
        if(NOT _mod EQUAL 0)
            message(FATAL_ERROR
                "vne_use_dep: CACHE_VARS expects an even number of arguments "
                "(VAR VALUE pairs), but got ${_len}")
        endif()
        # Restrict CACHE_VARS to boolean-only and validate values
        set(_vne_valid_bool_values TRUE FALSE ON OFF YES NO Y N 1 0)
        set(_i 0)
        while(_i LESS _len)
            list(GET ARG_CACHE_VARS ${_i} _var)
            math(EXPR _vi "${_i} + 1")
            if(_vi GREATER_EQUAL _len)
                break()
            endif()
            list(GET ARG_CACHE_VARS ${_vi} _val)
            # Validate that the value is a proper CMake boolean
            string(TOUPPER "${_val}" _val_upper)
            list(FIND _vne_valid_bool_values "${_val_upper}" _bool_idx)
            if(_bool_idx EQUAL -1)
                message(FATAL_ERROR
                    "vne_use_dep: CACHE_VARS entry '${_var}' must be a boolean "
                    "value (one of: TRUE, FALSE, ON, OFF, YES, NO, Y, N, 1, 0); "
                    "got '${_val}'")
            endif()
            set(${_var} ${_val} CACHE BOOL "" FORCE)
            math(EXPR _i "${_i} + 2")
        endwhile()
    endif()

    if(ARG_BINARY_DIR)
        add_subdirectory(${ARG_SUBDIR} ${ARG_BINARY_DIR})
    else()
        add_subdirectory(${ARG_SUBDIR})
    endif()
endfunction()
