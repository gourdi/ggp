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
target_link_libraries(ggocomputartbitmap ggocomputart)

add_executable(ggocomputartanim ggo_computart_anim.cpp)
target_link_libraries(ggocomputartanim ggocomputart)

add_executable(ggocomputartrealtime ggo_computart_realtime.cpp)
target_link_libraries(ggocomputartrealtime ggocomputart ${SDL2_LIBRARIES})
target_include_directories(ggocomputartrealtime PUBLIC ${SDL2_INCLUDE_DIRS})

