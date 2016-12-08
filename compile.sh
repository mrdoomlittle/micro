avr-g++ -c -std=c++11 -g -DF_CPU=16000000UL -Os -mmcu=atmega328p micro.cpp -o micro.o
avr-g++ -c -std=c++11 -g -DF_CPU=16000000UL -Os -mmcu=atmega328p wiring_digital.cpp -o wiring_digital.o
avr-g++ -g -mmcu=atmega328p micro.o wiring_digital.o -o micro.elf
avr-objcopy -j .text -j .data -O ihex micro.elf micro.hex
