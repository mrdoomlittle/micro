rm -f *.o
rm -f *.elf
rm -f *.hex
rm -f modules/*.o

C_IFLAGS="-I../tmp/inc -I../bci/inc -I../bci/8xdrm/inc -I/usr/local/include"
C_LFLAGS="-L../tmp/lib -L../bci/lib -L../bci/8xdrm/lib"
C_FLAGS="$C_IFLAGS $C_LFLAGS"
C_DEFINES="-D__WITH_TMP -D__WITH_BCI"
ARC="-DARC64"
F_CPU=
DEVICE=
if [ "$1" = "atmega328p" ]; then
	F_CPU=16000000UL
	DEVICE=atmega328p
elif [ "$1" = "attiny85" ]; then
	F_CPU=16500000UL
	DEVICE=attiny85
else
	echo "unknown arg"
	exit
fi


BOOT_LOADER=/home/daniel-robson/Desktop/micronucleus/firmware
BLHEX=main.hex
HEXMURGE=/home/daniel-robson/Projects/hexmurge


cd ../tmp;
make F_CPU=$F_CPU DEVICE=$DEVICE ARC=$ARC;
cd ../8xdrm;
make F_CPU=$F_CPU DEVICE=$DEVICE ARC=$ARC;
cd ../bci;
make F_CPU=$F_CPU DEVICE=$DEVICE ARC=$ARC;
cd ../micro;

avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o tmp_io.o tmp_io.c
avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o modules/eeprom.o modules/eeprom.c
avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o modules/24lc256.o modules/24lc256.c

#avr-gcc -c -g $ARC $CXX_IFLAGS -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o librarys/pic_prog.o librarys/pic_prog.c
avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o micro.o micro.c
avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o uart.o uart.c
avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o wiring_digital.o wiring_digital.c
avr-gcc -c -g -Wall $ARC $C_IFLAGS $C_DEFINES -std=c11 -D__$DEVICE -DF_CPU=$F_CPU -Os -mmcu=$DEVICE -o shift_reg.o shift_reg.c
avr-gcc -g -Wall $ARC $C_FLAGS $C_DEFINES -std=c11 -mmcu=$DEVICE -D__$DEVICE -DF_CPU=$F_CPU micro.o tmp_io.o uart.o wiring_digital.o shift_reg.o modules/eeprom.o modules/24lc256.o -o micro.elf -ltmp_io -lbci -l8xdrm
avr-objcopy -j .text -j .data -O ihex micro.elf micro.hex
if [ "$DEVICE" = "attiny85" ]; then
	cd $BOOT_LOADER;
	make clean;
	make;
	cd $HEXMURGE;
	./hexmurge "$BOOT_LOADER/$BLHEX" "../micro/micro.hex" "../micro/micro.hex"
fi
