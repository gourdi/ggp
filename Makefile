TARGETS=ggononreg ggokernel ggo2d ggoraytracer ggocomputart

CC=g++

export MODE

all: $(TARGETS)

ggononreg:
	$(MAKE) -C $@

ggokernel: ggononreg
	$(MAKE) -C $@

ggo2d: ggononreg ggokernel
	$(MAKE) -C $@

ggoraytracer: ggononreg ggokernel ggo2d
	$(MAKE) -C $@

ggocomputart: ggononreg ggokernel ggo2d ggoraytracer
	$(MAKE) -C $@

clean:
	$(MAKE) -C ggononreg clean
	$(MAKE) -C ggokernel clean
	$(MAKE) -C ggo2d clean
	$(MAKE) -C ggoraytracer clean
	$(MAKE) -C ggocomputart clean

.PHONY: $(TARGETS)