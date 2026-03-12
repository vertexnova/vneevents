#==============================================================================
# Doxygen.cmake
# Copyright (c) 2024 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#==============================================================================

if(VNE_DOXYGEN_MODULE_INCLUDED)
    return()
endif()
set(VNE_DOXYGEN_MODULE_INCLUDED TRUE)

function(enable_doxygen)
    option(ENABLE_DOXYGEN "Enable Doxygen documentation builds of source" OFF)

    if(ENABLE_DOXYGEN)
        set(DOXYGEN_CALLER_GRAPH YES)
        set(DOXYGEN_CALL_GRAPH YES)
        set(DOXYGEN_EXTRACT_ALL YES)
        set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

        find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

        if(Doxygen_FOUND)
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxyfile.in)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            message(STATUS "Doxygen build started...")

            add_custom_target(doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM
            )
        else()
            message(STATUS "Doxygen not found. Doxygen needs to be installed to generate the documentation.")
        endif()
    endif()
endfunction()
