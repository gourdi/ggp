include ../Makefile.config

CFLAGS+=-I. \
        -I./artists \
        -I./animation \
        -I./animation/paths \
        -I./animation/animates \
        -I../ggokernel/include \
        -I../ggo2d/include \
        -I../ggoraytracer/include
LIBDIR=lib
LIBS=../ggoraytracer/$(LIBDIR)/libggoraytracer.a ../ggo2d/$(LIBDIR)/libggo2d.a ../ggokernel/$(LIBDIR)/libggokernel.a
TARGETS=ggocomputartbitmaps ggocomputartanim

SRC=$(call rwildcard, animation, *.cpp) $(call rwildcard, artists, *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

all: $(TARGETS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

ggocomputartbitmaps: $(OBJ) ggo_computart_bitmaps.o $(LIBS)
	$(CC) $^ $(LDFLAGS) -o $@

ggocomputartanim: $(OBJ) ggo_computart_anim.o $(LIBS)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -fr $(TARGETS) $(OBJ) ggo_computart_bitmaps.o ggo_computart_anim.o

.PHONY: clean all

