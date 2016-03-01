include ../Makefile.config

CFLAGS+=-Isrc \
        -Isrc/renderers \
        -Isrc/raycasters \
        -Isrc/materials \
        -Isrc/fog \
        -Isrc/backgrounds \
        -Isrc/cameras \
        -Isrc/shaders \
        -I../ggononreg/include \
        -I../ggokernel/include \
        -I../ggo2d/include
LIBS=../ggononreg/$(LIBDIR)/libggononreg.a ../ggokernel/$(LIBDIR)/libggokernel.a ../ggo2d/$(LIBDIR)/libggo2d.a

HEADERS=src/ggo_object3d.h \
        src/ggo_scene.h \
        src/ggo_raytracer_global.h \
        src/raycasters/ggo_raycaster_abc.h \
        src/raycasters/ggo_brute_force_raycaster.h \
        src/renderers/ggo_renderer_abc.h \
        src/renderers/ggo_mono_sampling_renderer.h \
        src/renderers/ggo_antialiasing_renderer.h \
        src/renderers/ggo_global_sampling_renderer.h \
        src/renderers/ggo_indirect_lighting_abc.h \
        src/renderers/ggo_bidirectional_lighting.h \
        src/cameras/ggo_camera_abc.h \
        src/cameras/ggo_point_camera.h \
        src/cameras/ggo_isometric_camera.h \
        src/materials/ggo_material_abc.h \
        src/materials/ggo_solid_color_material.h \
        src/materials/ggo_checker_material.h \
        src/backgrounds/ggo_background3d_abc.h \
        src/backgrounds/ggo_background3d_color.h \
        src/fog/ggo_fog_abc.h \
        src/fog/ggo_linear_fog.h \
        src/fog/ggo_z_fog.h \
        src/shaders/ggo_shader_abc.h \
        src/shaders/ggo_phong_shader.h
LIB=$(LIBDIR)/libggoraytracer.a
OBJ=src/ggo_object3d.o \
    src/ggo_raytracer.o \
    src/ggo_scene.o \
    src/raycasters/ggo_raycaster_abc.o \
    src/raycasters/ggo_brute_force_raycaster.o \
    src/renderers/ggo_renderer_abc.o \
    src/renderers/ggo_mono_sampling_renderer.o \
    src/renderers/ggo_antialiasing_renderer.o \
    src/renderers/ggo_global_sampling_renderer.o \
    src/renderers/ggo_bidirectional_lighting.o \
    src/materials/ggo_solid_color_material.o \
    src/materials/ggo_checker_material.o \
    src/cameras/ggo_camera_abc.o \
    src/cameras/ggo_point_camera.o \
    src/cameras/ggo_isometric_camera.o \
    src/fog/ggo_linear_fog.o \
    src/fog/ggo_z_fog.o \
    src/shaders/ggo_phong_shader.o

NONREG_OBJ=test/ggo_raytracer_nonreg.o \
           test/ggo_scene1.o \
           test/ggo_scene2.o \
           test/ggo_scene3.o \
           test/ggo_scene4.o \
           test/ggo_scene5.o \
           test/ggo_scene6.o \
           test/ggo_scene7.o \
           test/ggo_marching_cubes_nonreg.o \
           test/ggo_polygon3d_nonreg.o \
           test/ggo_bidirectional_lighting.o \
           test/ggo_parallelogram3d_nonreg.o \
           test/ggo_transmission_nonreg.o
NONREG_EXE=./test/raytracer_nonreg  
    
all: $(LIB) $(NONREG_EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

check: $(NONREG_EXE)
	$(NONREG_EXE) $(NONREG_FILTER)
    
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

