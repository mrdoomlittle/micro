if [ "$1" = "" ]; then
	exit
fi

MICRO=/home/daniel-robson/Projects/micro
BOOT_LOADER=/home/daniel-robson/Desktop/micronucleus/firmware

if [ "$2" = "attiny85" ]; then
	cd $BOOT_LOADER;
	cp $MICRO/micro.hex main.hex;
	make fuse;
	make flash;
	exit
fi

if [ "$2" = "atmega328p" ]; then
	avrdude -C avrdude.conf -v -v -v -v -p atmega328p -c arduino -P/dev/tty"$1" -b 115200 -D -Uflash:w:micro.hex:i
fi
