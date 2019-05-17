project(ggp)

cmake_minimum_required(VERSION 3.8)

set(SDL2_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/3rdparty/SDL2-2.0.8/include")
set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/3rdparty/SDL2-2.0.8/lib/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rdparty/SDL2-2.0.8/lib/SDL2main.lib")

enable_testing()

add_subdirectory(3rdparty)
add_subdirectory(ggokernel)
add_subdirectory(ggo2d)
add_subdirectory(ggoanimation)
add_subdirectory(ggoraytracer)
add_subdirectory(ggophysics)
add_subdirectory(ggocomputart)
add_subdirectory(ggovivelle)
