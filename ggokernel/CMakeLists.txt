include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggokernel STATIC ${sources})
target_include_directories(ggokernel PUBLIC "src/")

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggokernel_nonreg ${sources})
target_link_libraries(ggokernel_nonreg ggokernel)
