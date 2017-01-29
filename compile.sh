rm -f *.o
rm -f *.elf
rm -f *.hex
avr-g++ -c -g -std=c++11 -DF_CPU=16000000UL -Os -mmcu=atmega328p micro.cpp -o micro.o
avr-g++ -c -g -std=c++11 -DF_CPU=16000000UL -Os -mmcu=atmega328p wiring_digital.cpp -o wiring_digital.o
#avr-g++ -c -g -DF_CPU=16000000UL -Os -mmcu=atmega328p memory.cpp -o memory.o
#avr-g++ -g -std=c++11 -mmcu=atmega328p micro.o wiring_digital.o /home/daniel-robson/Projects/tmp/io.o -o micro.elf
avr-g++ -g -std=c++11 -mmcu=atmega328p micro.o wiring_digital.o -o micro.elf
avr-objcopy -j .text -j .data -O ihex micro.elf micro.hex
