 include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
get_subdirectories("src" dirs)
group_sources(${sources})
add_library(ggoraytracer STATIC ${sources})
target_include_directories(ggoraytracer PUBLIC ${dirs})
target_link_libraries(ggoraytracer PUBLIC ggokernel ggo2d)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggoraytracer_nonreg ${sources})
target_link_libraries(ggoraytracer_nonreg ggokernel ggo2d ggoraytracer)

