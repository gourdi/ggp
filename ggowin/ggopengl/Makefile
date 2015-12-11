CC=clang++
CFLAGS=-std=c++11 -I../../ggokernel/test -I../../ggokernel/src \
       -I../../ggokernel/src/math -I../../ggokernel/src/math/shapes_2d \
       -I../../ggokernel/src/math/shapes_3d -I../../ggokernel/src/math/linear_algebra
LDFLAGS=-lpthread

ifeq ($(MODE),debug)
	CFLAGS+= -g -pg -DGGO_DEBUG
	LDFLAGS+= -pg
else
	CFLAGS+= -O3
endif

OS := $(shell uname)
ifeq ($(OS),Linux)
	CFLAGS+= -DLINUX
endif
ifeq ($(OS),Darwin)
	CFLAGS+= -DMACOS
endif
ifeq ($(OS),CYGWIN_NT-6.1)
	CFLAGS+= -DCYGWIN
    CC=g++
endif

NONREG_EXE=./ggopengl_nonreg

OBJ=ggo_matrix4x4.o ggo_camera.o

NONREG_OBJ=ggopengl_nonreg.o

all: $(NONREG_EXE)

check: $(NONREG_EXE)
	$(NONREG_EXE)
    
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
    
$(NONREG_EXE): $(OBJ) $(NONREG_OBJ)
	$(CC) $^ $(LDFLAGS) -o $(NONREG_EXE)
    
clean:
	rm -fr $(OBJ) $(NONREG_OBJ) $(NONREG_EXE)