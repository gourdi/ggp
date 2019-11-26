include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggophysics STATIC ${sources})
target_include_directories(ggophysics PUBLIC "src/")
target_link_libraries(ggophysics PUBLIC ggokernel)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggophysics_nonreg ${sources})
target_link_libraries(ggophysics_nonreg ggophysics)

# The UI executable.
get_sources_from_root_directory("ui" sources)
group_sources(${sources})
add_executable(ggophysics_ui ${sources})
target_link_libraries(ggophysics_ui ggophysics SDL2::SDL2 SDL2::SDL2main)

