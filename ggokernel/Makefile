include ../Makefile.config

CFLAGS+=-Isrc \
        -Isrc/nonreg \
        -Isrc/math \
        -Isrc/math/linear_algebra \
        -Isrc/math/interpolation \
        -Isrc/math/pcg \
        -Isrc/math/shapes_2d \
        -Isrc/math/shapes_3d \
        -Isrc/math/scalar_fields_2d \
        -Isrc/math/signal_proc \
        -Isrc/math/sampling \
        -Isrc/math/sampling/pixel_sampling \
        -Isrc/math/sampling/low_discrepancy_sequences \
        -Isrc/math/sampling/shape_sampling

NONREG_EXE=./test/kernel_nonreg
HALTON_TOOL=./tools/halton
BEST_CANDIDATE_TOOL=./tools/best_candidate

HEADERS=src/ggo_kernel.h \
        src/ggo_stdint.h \
        src/ggo_io.h \
        src/ggo_memory.h \
        src/ggo_helpers.h \
        src/ggo_array.h \
        src/ggo_array2d.h \
        src/ggo_tree.h \
        src/ggo_ease.h \
        src/ggo_link.h \
        src/ggo_best_match.h \
        src/ggo_data_fetcher.h \
        src/time/ggo_chronometer.h \
        src/math/ggo_harmonic_curve.h \
        src/math/ggo_harmonic_surface.h \
        src/math/ggo_set2.h \
        src/math/ggo_set3.h \
        src/math/ggo_distance2d.h \
        src/math/ggo_distance3d.h \
        src/math/ggo_quadratic.h \
        src/math/interpolation/ggo_curve.h \
        src/math/interpolation/ggo_interpolation1d.h \
        src/math/interpolation/ggo_interpolation2d.h \
        src/math/interpolation/ggo_interpolation3d.h \
        src/math/interpolation/ggo_triangle_interpolation.h \
        src/math/sampling/shape_sampling/ggo_shape_sampling.h \
        src/math/sampling/low_discrepancy_sequences/ggo_poisson_sampling.h \
        src/math/sampling/low_discrepancy_sequences/ggo_halton.h \
        src/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.h \
        src/math/sampling/pixel_sampling/ggo_pixel_sampler_abc.h \
        src/math/sampling/pixel_sampling/ggo_blur_pixel_sampler.h \
        src/math/sampling/pixel_sampling/ggo_shape_sampling_coef.h \
        src/math/delaunay_triangulation/ggo_delaunay_triangulation.h \
        src/math/marching_cubes/ggo_marching_cubes.h \
        src/math/linear_algebra/ggo_matrix.h src/math/linear_algebra/ggo_matrix.imp.h \
        src/math/linear_algebra/ggo_linear_algebra.h \
        src/math/linear_algebra/ggo_linear_algebra2d.h \
        src/math/linear_algebra/ggo_linear_algebra3d.h \
        src/math/linear_algebra/ggo_sparse_matrix.h \
        src/math/signal_proc/ggo_dct.h \
        src/math/signal_proc/ggo_dwt.h \
        src/math/signal_proc/ggo_fourier.h \
        src/math/signal_proc/ggo_gaussian_blur.h \
        src/math/signal_proc/ggo_box_filter.h \
        src/math/shapes_2d/ggo_shapes2d.h \
        src/math/shapes_2d/ggo_rect.h \
        src/math/shapes_2d/ggo_rect.imp.h \
        src/math/shapes_2d/ggo_circle.h \
        src/math/shapes_2d/ggo_circle.imp.h \
        src/math/shapes_2d/ggo_disc.h \
        src/math/shapes_2d/ggo_disc.imp.h \
        src/math/shapes_2d/ggo_segment.h \
        src/math/shapes_2d/ggo_segment.imp.h \
        src/math/shapes_2d/ggo_extended_segment.h \
        src/math/shapes_2d/ggo_extended_segment.imp.h \
        src/math/shapes_2d/ggo_spline.h \
        src/math/shapes_2d/ggo_spline.imp.h \
        src/math/shapes_2d/ggo_polygon.h \
        src/math/shapes_2d/ggo_polygon.imp.h \
        src/math/shapes_2d/ggo_triangle2d.h \
        src/math/shapes_2d/ggo_triangle2d.imp.h \
        src/math/shapes_2d/ggo_multi_shape.h \
        src/math/shapes_2d/ggo_multi_shape.imp.h \
        src/math/shapes_3d/ggo_ray3d.h \
        src/math/shapes_3d/ggo_segment3d.h \
        src/math/shapes_3d/ggo_triangle3d.h \
        src/math/shapes_3d/ggo_triangle3d.imp.h \
        src/math/shapes_3d/ggo_face3d.h \
        src/math/shapes_3d/ggo_face3d.imp.h \
        src/math/shapes_3d/ggo_axis_aligned_box3d.h \
        src/math/shapes_3d/ggo_axis_aligned_box3d.imp.h \
        src/math/shapes_3d/ggo_shapes3d.h \
        src/math/shapes_3d/ggo_basis3d.h \
        src/math/shapes_3d/ggo_basis3d.imp.h \
        src/math/shapes_3d/ggo_polygon3d.h \
        src/math/shapes_3d/ggo_polygon3d.imp.h \
        src/math/shapes_3d/ggo_centered_sphere3d.h \
        src/math/shapes_3d/ggo_centered_sphere3d.imp.h \
        src/math/shapes_3d/ggo_sphere3d.h \
        src/math/shapes_3d/ggo_sphere3d.imp.h \
        src/math/shapes_3d/ggo_plane3d.h \
        src/math/shapes_3d/ggo_plane3d.imp.h \
        src/math/shapes_3d/ggo_disc3d.h \
        src/math/shapes_3d/ggo_disc3d.imp.h \
        src/math/shapes_3d/ggo_parallelogram3d.h \
        src/math/shapes_3d/ggo_parallelogram3d.imp.h \
        src/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h \
        src/math/scalar_fields_2d/ggo_gaussian_field_2d.h \
        src/math/scalar_fields_2d/ggo_shape_field_2d.h \
        src/math/scalar_fields_2d/ggo_multi_scalar_field_2d.h \
        src/math/scalar_fields_2d/ggo_perlin_noise_field_2d.h
LIB=$(LIBDIR)/libggokernel.a
OBJ=src/ggo_kernel.o \
    src/ggo_color_stream.o \
    src/time/ggo_chronometer.o \
    src/nonreg/ggo_unit_test.o \
    src/nonreg/ggo_unit_test_manager.o \
    src/math/sampling/low_discrepancy_sequences/ggo_halton.o \
    src/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.o \
    src/math/sampling/pixel_sampling/ggo_pixel_sampler_abc.o \
    src/math/sampling/pixel_sampling/ggo_blur_pixel_sampler.o \
    src/math/sampling/pixel_sampling/ggo_shape_sampling_coef.o \
    src/math/signal_proc/ggo_dct.o \
    src/math/signal_proc/ggo_dwt.o \
    src/math/signal_proc/ggo_fourier.o \
    src/math/signal_proc/ggo_gaussian_blur.o \
    src/math/signal_proc/ggo_box_filter.o \
    src/math/delaunay_triangulation/ggo_delaunay_triangulation.o \
    src/math/marching_cubes/ggo_marching_cubes.o
NONREG_OBJ=test/ggo_kernel_nonreg.o \
           test/ggo_array_nonreg.o \
           test/ggo_array2d_nonreg.o \
           test/ggo_scalar_field2d_nonreg.o \
           test/ggo_base_nonreg.o \
           test/ggo_quadratic_nonreg.o \
           test/ggo_harmonics_nonreg.o \
           test/ggo_best_match_nonreg.o \
           test/interpolation/ggo_interpolation2d_nonreg.o \
           test/interpolation/ggo_interpolation3d_nonreg.o \
           test/signal_proc/ggo_dwt_nonreg.o \
           test/signal_proc/ggo_box_filter_nonreg.o \
           test/signal_proc/ggo_dct_nonreg.o \
           test/linear_algebra/ggo_matrix_nonreg.o \
           test/linear_algebra/ggo_sparse_matrix_nonreg.o \
           test/linear_algebra/ggo_linear_algebra2d_nonreg.o \
           test/linear_algebra/ggo_linear_algebra3d_nonreg.o \
           test/geometry/ggo_shapes2d_nonreg.o \
           test/geometry/ggo_shapes3d_nonreg.o \
           test/sampling/ggo_sampling_nonreg.o \
           test/sampling/ggo_pixel_sampling_nonreg.o

all: $(LIB) $(NONREG_EXE) $(HALTON_TOOL) $(BEST_CANDIDATE_TOOL)

check: $(NONREG_EXE)
	$(NONREG_EXE) $(NONREG_FILTER)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NONREG_EXE): $(OBJ) $(NONREG_OBJ)
	$(CC) $^ $(LDFLAGS) -o $(NONREG_EXE)

$(HALTON_TOOL): src/math/sampling/low_discrepancy_sequences/ggo_halton.o tools/halton.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
  
$(BEST_CANDIDATE_TOOL): src/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.o tools/best_candidate.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
  
$(LIB): $(OBJ)
	@mkdir -p include
	cp $(HEADERS) include
	@mkdir -p $(@D)
	ar rcs $@ $^

clean:
	rm -fr $(LIB) $(OBJ) include $(LIBDIR) $(NONREG_OBJ) $(NONREG_EXE) $(HALTON_TOOL) $(BEST_CANDIDATE_TOOL) gmon.out tools/*.txt

.PHONY: clean all
