include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
get_subdirectories("src" dirs)
group_sources(${sources})
add_library(ggo2d STATIC ${sources})
target_include_directories(ggo2d PUBLIC ${GGOKERNEL_INCLUDE} ${dirs})
target_link_libraries(ggo2d PUBLIC libtga ggokernel)
set(GGO2D_INCLUDE ${dirs} PARENT_SCOPE)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggo2d_nonreg ${sources})
target_link_libraries(ggo2d_nonreg ggo2d)

