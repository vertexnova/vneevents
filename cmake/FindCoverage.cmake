#==============================================================================
# Copyright (c) 2024 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# Author:    Ajeet Singh Yadav
# Created:   May-2024
#
# FindCoverage.cmake - Coverage settings for different platforms
#==============================================================================

if(VNE_COVERAGE_MODULE_INCLUDED)
    return()
endif()
set(VNE_COVERAGE_MODULE_INCLUDED TRUE)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    find_program(LCOV_EXECUTABLE lcov)
    find_program(GENHTML_EXECUTABLE genhtml)
    find_program(GCOV_EXECUTABLE gcov)

    if(NOT LCOV_EXECUTABLE)
        message(FATAL_ERROR "lcov not found! Please install it.")
    endif()
    if(NOT GENHTML_EXECUTABLE)
        message(FATAL_ERROR "genhtml not found! Please install it.")
    endif()
    if(NOT GCOV_EXECUTABLE)
        message(FATAL_ERROR "gcov not found! Please install it.")
    endif()
    if(UNIX AND NOT APPLE)
        message(STATUS "Configuring code coverage for Linux")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    elseif(APPLE)
        message(STATUS "Configuring code coverage for Apple")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
    elseif(WIN32)
        message(STATUS "Configuring code coverage for Windows")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /Ob0 /Od /RTC1")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /PROFILE")
        if(MSVC)
            include(FindProgram)
            find_program(LLVM_PROF llvm-profdata)
            if(LLVM_PROF)
                message(STATUS "LLVM profiler found")
            else()
                message(FATAL_ERROR "LLVM profiler not found. Install LLVM tools to enable coverage on Windows.")
            endif()
        else()
            message(FATAL_ERROR "Coverage on Windows requires MSVC with LLVM tools")
        endif()
    endif()

    if(LCOV_EXECUTABLE AND GENHTML_EXECUTABLE)
        if(NOT TARGET clean_coverage)
            add_custom_target(clean_coverage
                COMMAND ${LCOV_EXECUTABLE} --zerocounters --directory .
                COMMENT "Cleaning coverage counters"
                VERBATIM
            )
        endif()
        if(NOT TARGET coverage)
            add_custom_target(coverage
                COMMAND ${CMAKE_CTEST_COMMAND}
                COMMAND ${LCOV_EXECUTABLE} --capture --directory . --output-file coverage.info
                COMMAND ${LCOV_EXECUTABLE} --remove coverage.info '/usr/*' '*/tests/*' '*/3rd_party/*' '*/include/*' --output-file coverage_filtered.info
                COMMAND ${GENHTML_EXECUTABLE} coverage_filtered.info --output-directory coverage_report
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating code coverage report"
                VERBATIM
            )
        endif()
    else()
        message(WARNING "lcov and genhtml are required to generate coverage reports")
    endif()
endif()
