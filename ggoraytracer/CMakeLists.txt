 include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggoraytracer STATIC ${sources})
target_include_directories(ggoraytracer PUBLIC "src/")
target_link_libraries(ggoraytracer PUBLIC ggo2d)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggoraytracer_nonreg ${sources})
target_link_libraries(ggoraytracer_nonreg ggo2d ggoraytracer)

