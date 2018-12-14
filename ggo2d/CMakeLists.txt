include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggo2d STATIC ${sources})
target_include_directories(ggo2d PUBLIC "src/" PRIVATE ${JPEG_INCLUDE_DIR} ${PNG_INCLUDE_DIRS})
target_link_libraries(ggo2d PUBLIC ggokernel ${JPEG_LIBRARIES} ${PNG_LIBRARIES})
add_dependencies(ggo2d ggokernel_nonreg)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggo2d_nonreg ${sources})
target_link_libraries(ggo2d_nonreg ggo2d)
