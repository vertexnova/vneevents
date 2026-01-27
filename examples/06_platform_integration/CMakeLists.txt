#==============================================================================
# 06_platform_integration Example
#==============================================================================
# This example is only for desktop platforms (Windows, macOS, Linux)
# GLFW is not available on iOS/Web platforms
if(EMSCRIPTEN OR (APPLE AND (IOS OR ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")))
    message(STATUS "Skipping example_06_platform_integration (GLFW not available on ${CMAKE_SYSTEM_NAME})")
    return()
endif()

add_executable(example_06_platform_integration
    main.cpp
    platform_demo.cpp
    glfw_integration.cpp
)

# Add GLFW submodule
if(EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw ${CMAKE_BINARY_DIR}/external/glfw)
    set(GLFW_FOUND TRUE)
else()
    message(WARNING "GLFW submodule not found at external/glfw")
    set(GLFW_FOUND FALSE)
endif()

# Add GLAD library (opengl-glad submodule)
# Prefer glad::glad; fall back to vne::opengl::glad or system OpenGL
set(GLAD_TARGET "")
if(NOT TARGET glad::glad AND NOT TARGET vne::opengl::glad)
    if(EXISTS "${PROJECT_SOURCE_DIR}/libs/opengl-glad/CMakeLists.txt")
        # New standalone API: GLAD_ENABLE_* options, glad::glad target
        set(GLAD_ENABLE_GL ON)
        set(GLAD_ENABLE_GLES3 OFF)
        if(APPLE)
            set(GLAD_ENABLE_EGL OFF)
            set(GLAD_ENABLE_GLX OFF)
            set(GLAD_ENABLE_WGL OFF)
        elseif(WIN32)
            set(GLAD_ENABLE_EGL OFF)
            set(GLAD_ENABLE_GLX OFF)
            set(GLAD_ENABLE_WGL ON)
        else()
            set(GLAD_ENABLE_EGL ON)
            set(GLAD_ENABLE_GLX ON)
            set(GLAD_ENABLE_WGL OFF)
        endif()
        add_subdirectory(
            ${PROJECT_SOURCE_DIR}/libs/opengl-glad
            ${CMAKE_BINARY_DIR}/libs/opengl-glad
        )
        if(TARGET glad::glad)
            set(GLAD_TARGET glad::glad)
        elseif(TARGET vne::opengl::glad)
            set(GLAD_TARGET vne::opengl::glad)
        endif()
        message(STATUS "Using GLAD from libs/opengl-glad submodule")
    endif()
endif()
if(NOT GLAD_TARGET)
    if(TARGET glad::glad)
        set(GLAD_TARGET glad::glad)
    elseif(TARGET vne::opengl::glad)
        set(GLAD_TARGET vne::opengl::glad)
    else()
        message(WARNING "GLAD not found, falling back to system OpenGL")
        find_package(OpenGL REQUIRED)
    endif()
endif()

target_link_libraries(example_06_platform_integration
    PRIVATE
        vne::events
        vne::logging
        glfw
)

if(GLAD_TARGET)
    target_link_libraries(example_06_platform_integration PRIVATE ${GLAD_TARGET})
else()
    target_link_libraries(example_06_platform_integration PRIVATE OpenGL::GL)
endif()

# Include common utilities directory
target_include_directories(example_06_platform_integration
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(example_06_platform_integration PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
