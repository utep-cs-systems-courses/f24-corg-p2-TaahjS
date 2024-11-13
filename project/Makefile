COMMON_OBJECTS  = main.o 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include

CC              = msp430-elf-gcc
AS              = msp430-elf-as
AR              = msp430-elf-ar

all: led.elf
	(cd timerLib && make install)

#additional rules for files
led.elf: ${COMMON_OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ lib/libTimer.a

load: led.elf
	msp430loader.sh led.elf

clean:
	rm -f *.o *.elf *.a
	(cd timerLib && make clean)
	rm -rf h lib

main.o: led.h

wdInterruptHandler.o:  led.h
