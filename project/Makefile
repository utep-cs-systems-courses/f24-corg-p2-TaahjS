COMMON_OBJECTS  = main.o

CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -I../h

LDFLAGS         = -L/opt/ti/msp430_gcc/include



CC              = msp430-elf-gcc

AS              = msp430-elf-as

AR              = msp430-elf-ar



# Build both the timer library and main program

all: lib/libTimer.a led.elf



# Build led.elf only after lib/libTimer.a is created

led.elf: ${COMMON_OBJECTS} lib/libTimer.a

	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^



# Ensure timerLib/libTimer.a is built and installed before led.elf

lib/libTimer.a:

	(cd timerLib && make install)



# Load program onto the MSP430

load: led.elf

	msp430loader.sh led.elf



# Clean all generated files

clean:

	rm -f *.o *.elf *.a

	(cd timerLib && make clean)

	rm -rf h lib



main.o: led.h

wdInterruptHandler.o: led.h

