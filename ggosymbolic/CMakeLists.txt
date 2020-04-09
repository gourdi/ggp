include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggosymbolic STATIC ${sources})
add_dependencies(ggosymbolic ggokernel_nonreg)
target_include_directories(ggosymbolic PUBLIC "src/")
target_link_libraries(ggosymbolic PUBLIC ggokernel)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggosymbolic_nonreg ${sources})
target_link_libraries(ggosymbolic_nonreg ggosymbolic)
