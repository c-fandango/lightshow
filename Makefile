IDIR = include
CXX=g++
CFLAGS=-I$(IDIR) -pthread
XDIR=libs
ODIR=obj

_DEPS = functions.h canvas.h framebuffer-internal.h functions.h gpio-bits.h gpio.h hardware-mapping.h led-matrix.h multiplex-mappers-internal.h pixel-mapper.h thread.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = patterns.o functions.o framebuffer.o gpio.o led-matrix.o multiplex-mappers.o options-initialize.o pixel-mapper.o thread.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(XDIR)/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

patterns: $(OBJ)
	g++ -c libs/hardware-mapping.c -o obj/hardware-mapping.o
	$(CXX) -o $@ $^ obj/hardware-mapping.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
