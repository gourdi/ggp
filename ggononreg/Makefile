include ../Makefile.config

CFLAGS+=-Isrc

NONREG_EXE=./nonreg

HEADERS=src/ggo_nonreg.h src/ggo_unit_test.h src/ggo_unit_test_manager.h
LIB=$(LIBDIR)/libggononreg.a
OBJ=src/ggo_unit_test.o src/ggo_unit_test_manager.o
NONREG_OBJ=src/main.o

all: $(LIB) $(NONREG_EXE)

check: $(NONREG_EXE)
	$(NONREG_EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
    
$(NONREG_EXE): $(OBJ) $(NONREG_OBJ)
	$(CC) $^ $(LDFLAGS) -o $(NONREG_EXE)

$(LIB): $(OBJ)
	@mkdir -p include
	cp $(HEADERS) include
	@mkdir -p $(@D)
	ar rcs $@ $^

clean:
	rm -fr $(LIB) $(OBJ) include $(LIBDIR) $(NONREG_OBJ) $(NONREG_EXE)

.PHONY: clean all
