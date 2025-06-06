cmake_minimum_required(VERSION 4.0)
project(pt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern)

add_executable( ${PROJECT_NAME}
    src/utils/photon.h
    src/utils/ray.h
    src/utils/vec.h
    src/utils/defs.h
    src/utils/utils.h
    src/utils/sampler.h
    src/utils/pixel.h
    src/main.cpp
    src/lights/light.h
    src/lights/light.cpp
    src/render/prim.cpp
    src/render/scene.h
    src/render/window.cpp
    src/render/renderer.cpp
    src/render/prim.h
    src/render/intersection.h
    src/render/window.h
    src/render/renderer.h
    src/photonmap/kdtree.cpp
    src/photonmap/maxheap.h
    src/photonmap/photonmap.cpp
    src/photonmap/kdtree.h
    src/photonmap/photonmap.h
    src/photonmap/maxheap.cpp
    src/materials/bsdf.cpp
    src/materials/material.h
    src/materials/bsdf.h
    src/materials/material.cpp
    src/integrators/integrator.h
    src/integrators/photonintegrator.h
    src/integrators/photonintegrator.cpp
    src/integrators/integrator.cpp
    src/integrators/neeintegrator.cpp
    src/integrators/neeintegrator.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    src/utils
    src/lights
    src/render
    src/photonmap
    src/materials
    src/integrators
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE minifb::minifb)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wfatal-errors
    -Werror
    -Wno-unused-parameter 
)
