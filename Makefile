IDIR = include
CXX=g++
CFLAGS=-I$(IDIR) -pthread
XDIR=libs
ODIR=obj

_DEPS = functions.h canvas.h framebuffer-internal.h functions.h gpio-bits.h gpio.h hardware-mapping.h led-matrix.h multiplex-mappers-internal.h pixel-mapper.h thread.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = patterns.o functions.o framebuffer.o gpio.o led-matrix.o multiplex-mappers.o options-initialize.o pixel-mapper.o thread.o hardware-mapping.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(XDIR)/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(XDIR)/%.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

patterns: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
