cmake_minimum_required(VERSION 3.13)
project(assignment3starter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(viewflip2d)

include_directories(simulator)

#include_directories(glm)
#include_directories(glfw-3.3/include)
#include_directories(glew-2.1.0/include)
#
#set(GLEW_SRC glew-2.1.0/src/glew.c)

find_package(OpenGL REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.3)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

IF (WIN32)
    set(GLEW_DIR glew-2.1.0)
    include_directories(${GLEW_DIR}/include)
    add_definitions(-DGLEW_STATIC)
    link_directories(${GLEW_DIR}/lib)
ENDIF()

add_executable(assignment3starter
        viewflip2d/gluvi.cpp
        viewflip2d/gluvi.h
        viewflip2d/main.cpp
        viewflip2d/util.h
        viewflip2d/vec2.h
        simulator/Camera.h
        simulator/main.cpp
        simulator/shader.h
        array2.h
        array3.h
        grid.cpp
        grid.h
        main.cpp
        mainwithviewer.cpp
        particles.cpp
        particles.h
        shared_main.h
        util.h
        vec2.h
        vec3.h)


IF (WIN32)
    # Include glew32s if on windows
    target_link_libraries(${PROJECT_NAME} glfw glew32s glm ${OPENGL_LIBRARY})
ELSE()
    # don't include glew32s if not on windows
    target_link_libraries(${PROJECT_NAME} glfw glm ${OPENGL_LIBRARY})
ENDIF()