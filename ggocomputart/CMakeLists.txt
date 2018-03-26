include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("artists" artists_sources)
get_sources_from_root_directory("animation" animation_sources)
#get_subdirectories("artists" artists_dirs)
get_subdirectories("animation" animation_dirs)
group_sources(${artists_sources} ${animation_sources})
add_library(ggocomputart STATIC ${artists_sources} ${animation_sources})
target_include_directories(ggocomputart PUBLIC
  ${GGOKERNEL_INCLUDE}
  ${GGO2D_INCLUDE}
  ${GGOANIMATION_INCLUDE}
  ${GGORAYTRACER_INCLUDE}
  ${animation_dirs}
  "${CMAKE_CURRENT_SOURCE_DIR}/artists"
  "${CMAKE_CURRENT_SOURCE_DIR}")

# The executables.
add_executable(ggocomputartbitmap ggo_computart_bitmap.cpp)
target_link_libraries(ggocomputartbitmap ggokernel ggo2d ggoanimation ggoraytracer ggocomputart)

add_executable(ggocomputartanim ggo_computart_anim.cpp)
target_link_libraries(ggocomputartanim ggokernel ggo2d ggoanimation ggoraytracer ggocomputart)

add_executable(ggocomputartrealtime ggo_computart_realtime.cpp)
target_link_libraries(ggocomputartrealtime ggokernel ggo2d ggoanimation ggoraytracer ggocomputart)
target_include_directories(ggocomputartrealtime PUBLIC "../externals/SDL2-2.0.7/include")

