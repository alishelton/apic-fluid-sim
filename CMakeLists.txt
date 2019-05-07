cmake_minimum_required(VERSION 3.13)
project(assignment3starter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(viewflip2d)

include_directories(simulator)
include_directories(glm/include)
include_directories(glfw-3.3/include)
include_directories(glew-2.1.0/include)

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
