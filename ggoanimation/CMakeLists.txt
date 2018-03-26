include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
get_subdirectories("src" dirs)
group_sources(${sources})
add_library(ggoanimation STATIC ${sources})
target_include_directories(ggoanimation PUBLIC ${GGOKERNEL_INCLUDE} ${GGO2D_INCLUDE} ${dirs})
set(GGOANIMATION_INCLUDE ${dirs} PARENT_SCOPE)
