project(ggp)

cmake_minimum_required(VERSION 3.8)

enable_testing()

include("${PROJECT_BINARY_DIR}/conanbuildinfo.cmake")
link_directories(${CONAN_LIB_DIRS_LIBJPEG} ${CONAN_LIB_DIRS_LIBPNG} ${CONAN_LIB_DIRS_SDL2} ${CONAN_LIB_DIRS_GLFW} ${CONAN_LIB_DIRS_GLAD})

add_subdirectory(ggokernel)
add_subdirectory(ggo2d)
add_subdirectory(ggoanimation)
add_subdirectory(ggoraytracer)
add_subdirectory(ggophysics)
add_subdirectory(ggocomputart)
add_subdirectory(ggovivelle)
add_subdirectory(ggoprototype)
