# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

add_definitions(-D__ANDROID__ -DGL_GLEXT_PROTOTYPES)

set(GGP_SOURCE_DIR "../../../")
set(SDL2_SOURCE_DIR ${GGP_SOURCE_DIR}/externals/SDL2-2.0.4/)

include_directories(
  ${SDL2_SOURCE_DIR}/include
  ${SDL2_SOURCE_DIR}/src/render/opengles
  ${SDL2_SOURCE_DIR}/src/render/opengles2
  ${GGP_SOURCE_DIR}/ggokernel/src
  ${GGP_SOURCE_DIR}/ggokernel/src/threading
  ${GGP_SOURCE_DIR}/ggokernel/src/rle
  ${GGP_SOURCE_DIR}/ggokernel/src/math
  ${GGP_SOURCE_DIR}/ggokernel/src/math/marching_algorithms
  ${GGP_SOURCE_DIR}/ggokernel/src/math/delaunay_triangulation
  ${GGP_SOURCE_DIR}/ggokernel/src/math/signal_proc
  ${GGP_SOURCE_DIR}/ggokernel/src/math/interpolation
  ${GGP_SOURCE_DIR}/ggokernel/src/math/linear_algebra
  ${GGP_SOURCE_DIR}/ggokernel/src/math/scalar_fields_2d
  ${GGP_SOURCE_DIR}/ggokernel/src/math/sampling/shape_sampling
  ${GGP_SOURCE_DIR}/ggokernel/src/math/sampling/low_discrepancy_sequences
  ${GGP_SOURCE_DIR}/ggokernel/src/math/shapes_2d
  ${GGP_SOURCE_DIR}/ggokernel/src/math/shapes_3d
  ${GGP_SOURCE_DIR}/ggo2d/src
  ${GGP_SOURCE_DIR}/ggo2d/src/rle_image
  ${GGP_SOURCE_DIR}/ggo2d/src/paint
  ${GGP_SOURCE_DIR}/ggo2d/src/blur
  ${GGP_SOURCE_DIR}/ggo2d/src/fill
  ${GGP_SOURCE_DIR}/ggoraytracer/src
  ${GGP_SOURCE_DIR}/ggoraytracer/src/raycasters
  ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers
  ${GGP_SOURCE_DIR}/ggoraytracer/src/cameras
  ${GGP_SOURCE_DIR}/ggoraytracer/src/materials
  ${GGP_SOURCE_DIR}/ggoraytracer/src/backgrounds
  ${GGP_SOURCE_DIR}/ggoraytracer/src/fog
  ${GGP_SOURCE_DIR}/ggocomputart
  ${GGP_SOURCE_DIR}/ggocomputart/artists
  ${GGP_SOURCE_DIR}/ggocomputart/animation
  ${GGP_SOURCE_DIR}/ggocomputart/animation/paths)

file(GLOB SDL2_SOURCE_FILES
  ${SDL2_SOURCE_DIR}/src/*.c
  ${SDL2_SOURCE_DIR}/src/atomic/*.c
  ${SDL2_SOURCE_DIR}/src/audio/*.c
  ${SDL2_SOURCE_DIR}/src/audio/android/*.c
  ${SDL2_SOURCE_DIR}/src/audio/dummy/*.c
  ${SDL2_SOURCE_DIR}/src/core/android/*.c
  ${SDL2_SOURCE_DIR}/src/cpuinfo/*.c
  ${SDL2_SOURCE_DIR}/src/dynapi/*.c
  ${SDL2_SOURCE_DIR}/src/events/*.c
  ${SDL2_SOURCE_DIR}/src/file/*.c
  ${SDL2_SOURCE_DIR}/src/filesystem/android/*.c
  ${SDL2_SOURCE_DIR}/src/haptic/*.c
  ${SDL2_SOURCE_DIR}/src/haptic/dummy/*.c
  ${SDL2_SOURCE_DIR}/src/libm/*.c
  ${SDL2_SOURCE_DIR}/src/loadso/dlopen/*.c
  ${SDL2_SOURCE_DIR}/src/joystick/*.c
  ${SDL2_SOURCE_DIR}/src/joystick/android/*.c
  ${SDL2_SOURCE_DIR}/src/power/*.c
  ${SDL2_SOURCE_DIR}/src/power/android/*.c
  ${SDL2_SOURCE_DIR}/src/stdlib/*.c
  ${SDL2_SOURCE_DIR}/src/render/*.c
  ${SDL2_SOURCE_DIR}/src/render/opengles/*.c
  ${SDL2_SOURCE_DIR}/src/render/opengles2/*.c
  ${SDL2_SOURCE_DIR}/src/render/software/*.c
  ${SDL2_SOURCE_DIR}/src/thread/*.c
  ${SDL2_SOURCE_DIR}/src/thread/pthread/*.c
  ${SDL2_SOURCE_DIR}/src/timer/*.c
  ${SDL2_SOURCE_DIR}/src/timer/unix/*.c
  ${SDL2_SOURCE_DIR}/src/video/*.c
  ${SDL2_SOURCE_DIR}/src/video/android/*.c)

set(GGO_KERNEL_SOURCES_FILES
    ${GGP_SOURCE_DIR}/ggokernel/src/math/delaunay_triangulation/ggo_delaunay_triangulation.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/marching_algorithms/ggo_marching_cubes.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/marching_algorithms/ggo_marching_squares.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/sampling/low_discrepancy_sequences/ggo_halton.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/signal_proc/ggo_dct.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/signal_proc/ggo_dwt.cpp
    ${GGP_SOURCE_DIR}/ggokernel/src/math/signal_proc/ggo_fourier.cpp)

set(GGO_RAYTRACER_SOURCES_FILES
    ${GGP_SOURCE_DIR}/ggoraytracer/src/cameras/ggo_camera_abc.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/cameras/ggo_isometric_camera.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/cameras/ggo_point_camera.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/fog/ggo_linear_fog.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/fog/ggo_z_fog.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/ggo_object3d.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/ggo_raytracer.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/ggo_scene.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/materials/ggo_checker_material.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/materials/ggo_solid_color_material.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/raycasters/ggo_brute_force_raycaster.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/raycasters/ggo_raycaster_abc.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers/ggo_antialiasing_renderer.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers/ggo_bidirectional_lighting.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers/ggo_global_sampling_renderer.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers/ggo_mono_sampling_renderer.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers/ggo_photon_mapping.cpp
    ${GGP_SOURCE_DIR}/ggoraytracer/src/renderers/ggo_renderer_abc.cpp)

set(GGO_COMPUTART_SOURCES_FILES
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ggo_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ggo_bitmap_artist_abc.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ggo_animation_artist_abc.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/animation/ggo_animate_abc.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/animation/ggo_animator.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/animation/paths/ggo_linear_path.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/animation/paths/ggo_spiral_path.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/aggregation/ggo_aggregation_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/aggregation/ggo_aggregation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/alpha/ggo_alpha_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/alpha/ggo_alpha_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/amorosi/ggo_amorosi_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/bozons/ggo_bozons_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/bubbles/ggo_bubbles_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/buddhabrot/ggo_buddhabrot_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/cabrel/ggo_cabrel_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/cells/ggo_cells_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/chryzode/ggo_chryzode_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/chryzode/ggo_chryzode_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/chryzode/ggo_chryzode_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/circles/ggo_circles_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/circles/ggo_circles_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/circles/ggo_circles_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/crystal/ggo_crystal_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/crystal/ggo_crystal_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/crystal/ggo_crystal_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/cumbia/ggo_cumbia_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/cumbia/ggo_cumbia_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/cumbia/ggo_cumbia_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/distorsion/ggo_distorsion_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/distorsion/ggo_distorsion_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/duffing/ggo_duffing_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/duffing/ggo_duffing_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/dupecheck/ggo_dupecheck_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/dupecheck/ggo_dupecheck_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/entabeni/ggo_entabeni.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/entabeni/ggo_entabeni_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/entabeni/ggo_entabeni_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/filling_squares/ggo_filling_squares_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/filling_squares/ggo_filling_squares_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/filling_squares/ggo_filling_squares_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/flower/ggo_flower_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/hexa/ggo_hexa_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/hexa/ggo_hexa_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/hexa/ggo_hexa_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ifs/ggo_ifs_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ifs/ggo_ifs_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ifs/ggo_ifs_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/ikeda/ggo_ikeda_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/julia/ggo_julia_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/julia/ggo_julia_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/julia/ggo_julia_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/kanji/ggo_kanji_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/kanji/ggo_kanji_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/kanji/ggo_kanji_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/lagaude/ggo_lagaude_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/mandelbrot/ggo_mandelbrot_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/marbles/ggo_marbles_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/metaballs/ggo_metaballs_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/metaballs/ggo_metaballs_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/metaballs/ggo_metaballs_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/mondrian/ggo_mondrian_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/mosaic/ggo_mosaic_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/neon/ggo_neon_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/newton/ggo_newton_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/plastic/ggo_plastic_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/plastic/ggo_plastic_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/plastic/ggo_plastic_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/rah/ggo_rah_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/rediff/ggo_rediff_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/rex/ggo_rex_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/smoke/ggo_smoke_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/sonson/ggo_sonson_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/stoa/ggo_stoa_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/stoa/ggo_stoa_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/stoa/ggo_stoa_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/storni/ggo_storni_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/topodoko/ggo_topodoko_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/toutouyoutou/ggo_toutouyoutou_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/toutouyoutou/ggo_toutouyoutou_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/trees/ggo_trees_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/voronoi/ggo_voronoi_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/vortex/ggo_vortex_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/vortex2/ggo_vortex2_animation_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/vortex2/ggo_vortex2_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/vortex2/ggo_vortex2_bitmap_artist.cpp
    ${GGP_SOURCE_DIR}/ggocomputart/artists/worms/ggo_worms_artist.cpp)

add_library(SDL2 SHARED ${SDL2_SOURCE_FILES})
add_library(ggokernel SHARED ${GGO_KERNEL_SOURCES_FILES})
add_library(ggoraytracer SHARED ${GGO_RAYTRACER_SOURCES_FILES})
add_library(ggocomputart SHARED ${GGO_COMPUTART_SOURCES_FILES})
add_library(main SHARED ${GGP_SOURCE_DIR}/ggocomputart/ggocomputartrealtime.cpp src/main/cpp/ggocomputart/SDL_android_main.c)

find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv1_CM-lib GLESv1_CM)
find_library(GLESv2-lib GLESv2)

target_link_libraries(SDL2 ${log-lib} ${android-lib} ${EGL-lib} ${GLESv1_CM-lib} ${GLESv2-lib})
target_link_libraries(ggocomputart ggokernel ggoraytracer)
target_link_libraries(main ggokernel ggocomputart SDL2)
