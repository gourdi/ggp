include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggo2d STATIC ${sources})
target_include_directories(ggo2d PUBLIC "src/")
add_dependencies(ggo2d ggokernel_nonreg)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
target_link_libraries(ggo2d PUBLIC ggokernel PRIVATE ${JPEG_LIBRARIES} ${PNG_LIBRARIES})

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggo2d_nonreg ${sources})
target_link_libraries(ggo2d_nonreg ggo2d)
