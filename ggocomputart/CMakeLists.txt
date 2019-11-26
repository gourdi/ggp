include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggocomputart STATIC ${sources})
target_include_directories(ggocomputart PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(ggocomputart ggokernel ggo2d ggoraytracer ggoanimation ggophysics)
add_dependencies(ggocomputart ggoraytracer_nonreg ggoanimation_nonreg)

# The executables.
add_executable(ggocomputartbitmap ggo_computart_bitmap.cpp)
target_link_libraries(ggocomputartbitmap PRIVATE ggocomputart)

add_executable(ggocomputartanimation ggo_computart_animation.cpp)
target_link_libraries(ggocomputartanimation PRIVATE ggocomputart)

add_executable(ggocomputartrealtime ggo_computart_realtime.cpp)
target_link_libraries(ggocomputartrealtime PRIVATE ggocomputart SDL2::SDL2 SDL2::SDL2main)

add_executable(ggocomputartgpu ggo_computart_gpu.cpp)
target_link_libraries(ggocomputartgpu PRIVATE ggocomputart glad::glad glfw)


