include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggo2d STATIC ${sources})
add_dependencies(ggo2d ggokernel_nonreg)
target_include_directories(ggo2d PUBLIC "src/" PRIVATE ${JPEG_INCLUDE_DIR})
if (APPLE)
  set(LIBPNG "png-static" PARENT_SCOPE)
else()
  set(LIBPNG "png" PARENT_SCOPE)
endif()
target_link_libraries(ggo2d PUBLIC ggokernel PRIVATE ${LIBPNG} ${JPEG_LIBRARIES})

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggo2d_nonreg ${sources})
target_link_libraries(ggo2d_nonreg ggo2d)
