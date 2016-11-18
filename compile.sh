avr-g++ -c -g -DF_CPU=16000000UL -Os -mmcu=atmega328p main.cpp -o main.o
avr-g++ -g -mmcu=atmega328p -o main.elf main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
