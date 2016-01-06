include ../Makefile.config

CFLAGS+=-Isrc \
        -Isrc/io \
        -Isrc/color \
        -Isrc/brush \
        -Isrc/blender \
        -Isrc/sampling \
        -Isrc/image_data \
        -Isrc/paint \
        -Isrc/space_partitionning \
        -I../ggononreg/include \
        -I../ggokernel/include
LIBS=../ggononreg/$(LIBDIR)/libggononreg.a ../ggokernel/$(LIBDIR)/libggokernel.a

NONREG_EXE=./test/2d_nonreg

HEADERS=src/ggo_2d.h \
        src/ggo_fill.h \
        src/ggo_layer.h \
        src/ggo_pixel_rect.h \
        src/brush/ggo_brush_abc.h \
        src/brush/ggo_gradient_brush.h \
        src/brush/ggo_triangle_interpolation_brush.h \
        src/blender/ggo_blender_abc.h \
        src/image_data/ggo_image_data_abc.h \
        src/image_data/ggo_gray_image_data.h \
        src/image_data/ggo_rgb_image_data.h \
        src/paint/ggo_paint_helpers.h \
        src/paint/ggo_paint.h \
        src/paint/ggo_paint_params.h \
        src/paint/ggo_soft_paint.h \
        src/paint/ggo_seed_paint.h \
        src/paint/ggo_hard_paint.h \
        src/paint/ggo_shapes_collection.h \
        src/paint/ggo_shapes_collection.imp.h \
        src/color/ggo_color.h \
        src/color/ggo_color_conv.h \
        src/processing/ggo_morphology.h \
        src/space_partitionning/ggo_space_partitionning_2d_abc.h \
        src/space_partitionning/ggo_recursive_space_partitionning_2d.h \
        src/space_partitionning/ggo_basic_space_partitionning_2d.h \
        src/space_partitionning/ggo_pixel_space_partitionning_2d.h \
        src/io/ggo_bmp.h  
LIB=$(LIBDIR)/libggo2d.a
OBJ=src/ggo_fill.o \
    src/ggo_layer.o \
    src/ggo_pixel_rect.o \
    src/image_data/ggo_gray_image_data.o \
    src/image_data/ggo_rgb_image_data.o \
    src/paint/ggo_paint_helpers.o \
    src/paint/ggo_soft_paint.o \
    src/paint/ggo_seed_paint.o \
    src/paint/ggo_hard_paint.o \
    src/space_partitionning/ggo_space_partitionning_2d_abc.o \
    src/space_partitionning/ggo_recursive_space_partitionning_2d.o \
    src/space_partitionning/ggo_basic_space_partitionning_2d.o \
    src/space_partitionning/ggo_pixel_space_partitionning_2d.o \
    src/processing/ggo_morphology.o \
    src/io/ggo_bmp.o \
    src/color/ggo_color.o \
    src/color/ggo_color_conv.o
NONREG_OBJ=test/ggo_2d_nonreg.o \
           test/ggo_paint_nonreg.o \
           test/ggo_space_partitionning_nonreg.o \
           test/ggo_box_filter_nonreg.o \
           test/ggo_color_nonreg.o \
           test/ggo_layer_nonreg.o \
           test/ggo_shape_sampling_nonreg.o \
           test/ggo_low_discrepancy_sequences_nonreg.o \
           test/ggo_harmonic_surface_nonreg.o \
           test/ggo_interpolation_nonreg.o \
           test/ggo_triangular_interpolation_nonreg.o

all: $(LIB) $(NONREG_EXE)

check: $(NONREG_EXE)
	$(NONREG_EXE) $(NONREG_FILTER)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
    
$(NONREG_EXE): $(OBJ) $(NONREG_OBJ) $(LIBS)
	$(CC) $^ $(LDFLAGS) -o $(NONREG_EXE)

$(LIB): $(OBJ)
	@mkdir -p include
	cp $(HEADERS) include
	@mkdir -p $(@D)
	ar rcs $@ $^

clean:
	rm -fr $(LIB) $(OBJ) include $(LIBDIR) $(NONREG_OBJ) $(NONREG_EXE)

.PHONY: clean all
