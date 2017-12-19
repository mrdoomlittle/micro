rm -f *.o
rm -f *.elf
rm -f *.hex
rm -f modules/*.o drivers/*.o

build_tmp=0
build_bci=0
build_mfs=0

inc_flags="-Imdlint/inc -I/usr/local/include"
lib_flags=""
defines=""
arc="arc64"
f_cpu=
device=
if [ "$1" = "atmega328p" ]; then
	f_cpu=16000000UL
	device=atmega328p
elif [ "$1" = "attiny85" ]; then
	f_cpu=16500000UL
	device=attiny85
else
	echo "unknown arg"
	exit
fi


BOOT_LOADER=/home/daniel-robson/Desktop/micronucleus/firmware
BLHEX=main.hex
HEXMURGE=/home/daniel-robson/Projects/hexmurge
objs="uart.o wiring_digital.o shift_reg.o modules/eeprom.o drivers/24lc256.o"
ld_flags=""

if [ $build_tmp -eq 1 ]; then
	cd tmp;
	sh clean.sh
	make f_cpu=$f_cpu device=$device arc=$arc;
	cd ../;
	inc_flags="$inc_flags -Itmp/inc"
	lib_flags="$lib_flags -Ltmp/lib"
	ld_flags="$ld_flags -lmdl-tmp_io"
	defines="$defines -D__mdl_tmp -D__TMP_LIGHT"
fi

if [ $build_bci -eq 1 ]; then
	cd bci
	sh clean.sh
	make f_cpu=$f_cpu device=$device arc=$arc;
	cd ../;
	inc_flags="$inc_flags -Ibci/inc -Ibci/bitct/inc"
	lib_flags="$lib_flags -Lbci/lib -Lbci/bitct/lib"
	ld_flags="$ld_flags -lmdl-bci -lmdl-bitct"
	defines="$defines -D__mdl_bci"
fi

if [ $build_mfs -eq 1 ]; then
	cd mfs;
	sh clean.sh
	make f_cpu=$f_cpu device=$device arc=$arc;
	cd ../;
	inc_flags="$inc_flags -Imfs/inc"
	lib_flags="$lib_flags -Lmfs/lib"
	ld_flags="$ld_flags -lmdl-mfs"
	defines="$defines -D__mdl_mfs"
fi

flags="$inc_flags $lib_flags"
cd ../micro;

if [ $build_tmp -eq 1 ]; then
avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o tmp_io.o tmp_io.c
objs="$objs tmp_io.o"
fi

avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o modules/eeprom.o modules/eeprom.c
avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o drivers/24lc256.o drivers/24lc256.c

#avr-gcc -c -g $arc $CXX_IFLAGS -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o librarys/pic_prog.o librarys/pic_prog.c
avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o micro.o micro.c
avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o uart.o uart.c
avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o wiring_digital.o wiring_digital.c
avr-gcc -c -g -Wall -D__$arc $inc_flags $defines -std=c11 -D__$device -DF_CPU=$f_cpu -Os -mmcu=$device -o shift_reg.o shift_reg.c
avr-gcc -g -Wall -D__$arc $flags $defines -std=c11 -mmcu=$device -D__$device -DF_CPU=$f_cpu micro.o $objs -o micro.elf $ld_flags
avr-objcopy -j .text -j .data -O ihex micro.elf micro.hex
if [ "$device" = "attiny85" ]; then
	cd $BOOT_LOADER;
	make clean;
	make;
	cd $HEXMURGE;
	./hexmurge "$BOOT_LOADER/$BLHEX" "../micro/micro.hex" "../micro/micro.hex"
fi
