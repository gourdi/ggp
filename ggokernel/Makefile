include ../Makefile.config

CFLAGS+=-Isrc \
        -Isrc/rle \
        -Isrc/nonreg \
        -Isrc/rle \
        -Isrc/math \
        -Isrc/math/delaunay_interpolation \
        -Isrc/math/interpolation \
        -Isrc/math/linear_algebra \
        -Isrc/math/marching_cubes \
        -Isrc/math/sampling \
        -Isrc/math/sampling/pixel_sampling \
        -Isrc/math/sampling/low_discrepancy_sequences \
        -Isrc/math/sampling/shape_sampling \
        -Isrc/math/scalar_fields_2d \
        -Isrc/math/shapes_2d \
        -Isrc/math/shapes_3d \
        -Isrc/math/signal_proc

HALTON_TOOL=./tools/halton
BEST_CANDIDATE_TOOL=./tools/best_candidate

HEADERS=$(wildcard src/*.h) \
        $(wildcard src/time/*.h) \
        $(wildcard src/nonreg/*.h) \
        $(wildcard src/rle/*.h) \
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
        $(wildcard src/math/signal_proc/*.h) \
        $(wildcard src/math/shapes_2d/*.h) \
        $(wildcard src/math/shapes_3d/*.h) \
        $(wildcard src/math/scalar_fields_2d/*.h)
LIB=$(LIBDIR)/libggokernel.a

SRC=$(call rwildcard, src/, *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

NONREG_SRC=$(call rwildcard, test/, *.cpp)
NONREG_OBJ=$(patsubst %.cpp, %.o, $(NONREG_SRC))
NONREG_EXE=./test/kernel_nonreg

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
