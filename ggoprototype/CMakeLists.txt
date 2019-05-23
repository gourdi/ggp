include(../cmake/ggp.cmake)

add_executable(ggoprototype main.cpp)
target_link_libraries(ggoprototype ggokernel ggo2d ggoraytracer ggoanimation)

