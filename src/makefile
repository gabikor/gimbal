IDIR =../include
CC=g++
CFLAGS=-pthread#-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

# _DEPS = MPU6050.h I2cPort.h

_OBJ = main.o MPU6050.o I2cPort.o ramdiskWriter.o positioningData.o gpio.o motorControl.o pwmGenerator.o ThreePhasePWM.o gpio2.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp #$(_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gimbal: $(OBJ)
	$(CC) -Wall -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
