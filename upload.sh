avrdude -C avrdude.conf -v -v -v -v -p atmega328p -c arduino -P/dev/ttyUSB0 -b 115200 -D -Uflash:w:main.hex:i
