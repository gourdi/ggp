include ../Makefile.config

CFLAGS+=-Isrc \
        -Isrc/renderers \
        -Isrc/raycasters \
        -Isrc/materials \
        -Isrc/fog \
        -Isrc/backgrounds \
        -Isrc/cameras \
        -I../ggononreg/include \
        -I../ggokernel/include \
        -I../ggo2d/include
LIBS=../ggokernel/$(LIBDIR)/libggokernel.a ../ggo2d/$(LIBDIR)/libggo2d.a

HEADERS=src/ggo_object3d.h \
        src/ggo_scene.h \
        src/ggo_raytracer_global.h \
        $(wildcard src/raycasters/*.h) \
        $(wildcard src/renderers/*.h) \
        $(wildcard src/cameras/*.h) \
        $(wildcard src/materials/*.h) \
        $(wildcard src/backgrounds/*.h) \
        $(wildcard src/fog/*.h)
LIB=$(LIBDIR)/libggoraytracer.a

SRC=$(call rwildcard, src/, *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

NONREG_SRC=$(call rwildcard, test/, *.cpp)
NONREG_OBJ=$(patsubst %.cpp, %.o, $(NONREG_SRC))
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

