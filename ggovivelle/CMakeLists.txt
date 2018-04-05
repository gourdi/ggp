include(../cmake/ggp.cmake)

get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_executable(vivelle ${sources})
target_link_libraries(vivelle ggo2d)