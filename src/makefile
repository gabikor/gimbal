IDIR =../include
CC=g++
CFLAGS=#-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

# _DEPS = MPU6050.h I2cPort.h

_OBJ = main.o MPU6050.o I2cPort.o ramdiskWriter.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp #$(_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gimbal: $(OBJ)
	$(CC) -Wall -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
