include(../cmake/ggp.cmake)

# The static library.
add_sources_from(
  "src"
  "src/math"
  "src/math/shapes_2d"
  "src/math/shapes_3d"
  "src/math/delaunay_triangulation"
  "src/math/interpolation"
  "src/math/marching_algorithms"
  "src/math/sampling"
  "src/math/sampling/shape_sampling"
  "src/math/sampling/low_discrepancy_sequences"
  "src/math/scalar_fields_2d"
  "src/math/signal_proc"
  "src/math/linear_algebra"
  "src/memory"
  "src/nonreg"
  "src/rle"
  "src/threading"
  "src/time"
  "src/trees")
add_library(ggokernel STATIC ${src})

# The non-regression executable.
add_sources_from(
  "test"
  "test/interpolation"
  "test/linear_algebra"
  "test/memory"
  "test/rle"
  "test/sampling"
  "test/shapes2d"
  "test/shapes3d"
  "test/signal_proc"
  "test/template"
  "test/trees")
add_executable(ggokernel_nonreg ${src})
target_link_libraries(ggokernel_nonreg ggokernel)