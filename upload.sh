avrdude -C avrdude.conf -v -v -v -v -p atmega328p -c arduino -P/dev/tty"$1" -b 115200 -D -Uflash:w:micro.hex:i #-U lfuse:w:0x62:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m -U lock:w:0xFF:m 
