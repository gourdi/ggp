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

HEADERS=src/ggo_fill.h \
        src/ggo_layer.h \
        src/ggo_pixel_rect.h \
        $(wildcard src/image/*.h) \
        $(wildcard src/blender/*.h) \
        $(wildcard src/image/*.h) \
        $(wildcard src/paint/*.h) \
        $(wildcard src/color/*.h) \
        $(wildcard src/io/*.h)  
LIB=$(LIBDIR)/libggo2d.a

SRC=$(call rwildcard, src/, *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

NONREG_SRC=$(call rwildcard, test/, *.cpp)
NONREG_OBJ=$(patsubst %.cpp, %.o, $(NONREG_SRC))
NONREG_EXE=./test/2d_nonreg

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
