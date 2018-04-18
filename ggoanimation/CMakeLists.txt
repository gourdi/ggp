include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggoanimation STATIC ${sources})
target_include_directories(ggoanimation PUBLIC "src/")
target_link_libraries(ggoanimation ggokernel ggo2d)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggoanimation_nonreg ${sources})
target_link_libraries(ggoanimation_nonreg ggokernel ggo2d ggoanimation)

