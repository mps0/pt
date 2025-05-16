cmake_minimum_required(VERSION 4.0)
project(pt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern)

add_executable( ${PROJECT_NAME}
    src/defs.h
    src/integrator.cpp
    src/integrator.h
    src/intersection.h
    src/kdtree.h
    src/light.cpp
    src/light.h
    src/main.cpp
    src/material.cpp
    src/material.h
    src/pixel.h
    src/prim.cpp
    src/prim.h
    src/ray.h
    src/renderer.cpp
    src/renderer.h
    src/sampler.h
    src/scene.h
    src/utils.h
    src/vec.h
    src/window.cpp
    src/window.h
)
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE minifb::minifb)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wfatal-errors
)
