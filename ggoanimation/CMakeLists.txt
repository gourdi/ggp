include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggoanimation STATIC ${sources})
add_dependencies(ggoanimation ggo2d_nonreg)
target_include_directories(ggoanimation PUBLIC "src/")
target_link_libraries(ggoanimation PUBLIC ggo2d PRIVATE tinyxml2::tinyxml2)

# The non-regression executable.
get_sources_from_root_directory("test" sources)
group_sources(${sources})
add_executable(ggoanimation_nonreg ${sources})
target_link_libraries(ggoanimation_nonreg ggoanimation)

# The player executable.
add_executable(ggoanimation_player "player/ggo_animation_player.cpp")
target_link_libraries(ggoanimation_player PRIVATE ggoanimation SDL2::SDL2 SDL2::SDL2main)


