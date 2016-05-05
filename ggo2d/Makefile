include ../Makefile.config

CFLAGS+=-Isrc \
        -Isrc/blender \
        -Isrc/brush \
        -Isrc/color \
        -Isrc/image \
        -Isrc/io \
        -Isrc/paint \
        -I../ggokernel/include
LIBS=../ggokernel/$(LIBDIR)/libggokernel.a

NONREG_EXE=./test/2d_nonreg

HEADERS=src/ggo_fill.h \
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
        src/color/ggo_color.h \
        src/color/ggo_color_conv.h \
        src/processing/ggo_morphology.h \
        src/io/ggo_bmp.h  
LIB=$(LIBDIR)/libggo2d.a

SRC=$(call rwildcard, src/, *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

NONREG_SRC=$(call rwildcard, test/, *.cpp)
NONREG_OBJ=$(patsubst %.cpp, %.o, $(NONREG_SRC))

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
