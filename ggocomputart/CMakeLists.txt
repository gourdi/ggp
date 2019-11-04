include(../cmake/ggp.cmake)

# The static library.
get_sources_from_root_directory("src" sources)
group_sources(${sources})
add_library(ggocomputart STATIC ${sources})
target_include_directories(ggocomputart PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(ggocomputart ggokernel ggo2d ggoraytracer ggoanimation ggophysics)
add_dependencies(ggocomputart ggoraytracer_nonreg ggoanimation_nonreg)

# The executables.
add_executable(ggocomputartbitmap ggo_computart_bitmap.cpp)
target_link_libraries(ggocomputartbitmap PRIVATE ggocomputart)

add_executable(ggocomputartanim ggo_computart_anim.cpp)
target_link_libraries(ggocomputartanim PRIVATE ggocomputart)

add_executable(ggocomputartrealtime ggo_computart_realtime.cpp)
target_include_directories(ggocomputartrealtime PRIVATE ${CONAN_INCLUDE_DIRS_SDL2})
target_link_libraries(ggocomputartrealtime PRIVATE ggocomputart ${CONAN_LIBS_SDL2})

add_executable(ggocomputartgpu ggo_computart_gpu.cpp)
target_include_directories(ggocomputartgpu PRIVATE ${CONAN_INCLUDE_DIRS_GLFW} ${CONAN_INCLUDE_DIRS_GLAD})
target_link_libraries(ggocomputartgpu PRIVATE ggocomputart ${CONAN_LIBS_GLFW} ${CONAN_LIBS_GLAD} opengl32)


