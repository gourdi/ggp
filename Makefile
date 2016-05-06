TARGETS=ggokernel ggo2d ggoraytracer ggocomputart

CC=g++

export MODE

all: $(TARGETS)

ggokernel:
	$(MAKE) -C $@

ggo2d: ggokernel
	$(MAKE) -C $@

ggoraytracer: ggokernel ggo2d
	$(MAKE) -C $@

ggocomputart: ggokernel ggo2d ggoraytracer
	$(MAKE) -C $@

clean:
	$(MAKE) -C ggokernel clean
	$(MAKE) -C ggo2d clean
	$(MAKE) -C ggoraytracer clean
	$(MAKE) -C ggocomputart clean

.PHONY: $(TARGETS)
