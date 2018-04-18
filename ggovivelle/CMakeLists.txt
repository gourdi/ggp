include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggovivelle STATIC ${sources})
target_link_libraries(ggovivelle ggo2d)
get_subdirectories("src" dirs)
target_include_directories(ggovivelle PUBLIC "src/")

# The main executable.
add_executable(vivelle ggovivelle.cpp)
target_link_libraries(vivelle ggovivelle)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggovivelle_nonreg ${sources})
target_link_libraries(ggovivelle_nonreg ggovivelle)
