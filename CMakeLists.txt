cmake_minimum_required(VERSION 3.14.5)

project(ggp)

find_package(JPEG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_subdirectory(ggokernel)
add_subdirectory(ggo2d)
add_subdirectory(ggoanimation)
add_subdirectory(ggoraytracer)
add_subdirectory(ggophysics)
add_subdirectory(ggocomputart)
add_subdirectory(ggovivelle)
add_subdirectory(ggosymbolic)
