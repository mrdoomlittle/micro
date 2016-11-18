#include <avr/io.h>
#include <util/delay.h>
# include "binary.h"
# include <stdint.h>
# define pstate_high 0x1
# define pstate_low 0x0

// ignore the code i was just doing somthing for fun
//typedef boost::uint8_t uint8_t;
void digital_write(uint8_t pid, uint8_t pstate)
{
    if (pid > 5) return;
    uint8_t nlen = 5;
    uint8_t val = 0;

    if (pstate == 0x1) val |= 1 << pid;
    if (pstate == 0x0) val &= ~(1 << pid);
    PORTB = val;

    //PORTB = ((nlen - pid) >> PORTB) & pstate;
}

uint8_t digital_read(uint8_t pid)
{
    if (pid > 5) return 0;
    uint8_t o = PINB;
    return (((o >> pid) & 1) == 1? 1 : 0);
}

void delay_(int ms)
{
  while (0 < ms)
  {  
    _delay_ms(0.05);
    --ms;
  }

}
# define OD 0
# define OC 1
# define OL 2

# define ID 3
# define IC 4
# define IL 5

# define del 10
void set_osreg(uint8_t bits[8])
{
    for(uint8_t i = 0; i != 8; i++)
    {
        digital_write(OD, bits[i]);
        digital_write(OC, pstate_high);
        delay_(del);
        digital_write(OC, pstate_low);
        delay_(del);
    }
    digital_write(OL, pstate_high);
    delay_(del);
    digital_write(OL, pstate_low);
}

uint8_t * get_isreg()
{
    digital_write(IL, pstate_high);
    delay_(del);
    digital_write(IL, pstate_low);
    static uint8_t bits[8];
    for(uint8_t i = 0; i != 8; i++)
    {
        bits[i] = digital_read(ID);
        digital_write(IC, pstate_high);
        delay_(del);
        digital_write(IC, pstate_low);
        delay_(del);

        //bits[i] = digital_read(ID);
    }
  
    return bits;
}

uint8_t * num_to_arr(uint8_t num)
{
    static uint8_t arr[8];
    for(uint8_t i = 0; i != 8; i++)
    {
       arr[i] = ((num >> i) & 1) == 1? 1 : 0;  
    }
    return arr;
}
uint8_t arr_to_num(uint8_t * arr)
{
    uint8_t output = 0;
    for(uint8_t i = 0; i != 8; i++)
    {
        if (arr[i] == 1) output ^= 1 << i;        
    }

    return output;
}

struct tmp_memory
{
    tmp_memory operator=(uint8_t mem)
    {
        set_osreg(num_to_arr(mem));
    }
    tmp_memory operator+=(uint8_t amount)
    {
        uint8_t nex_nm = arr_to_num(get_isreg()) + amount;
        set_osreg(num_to_arr(nex_nm)); 
    }
    tmp_memory operator-=(uint8_t amount)
    {
        uint8_t nex_nm = arr_to_num(get_isreg()) - amount;
        set_osreg(num_to_arr(nex_nm));
    }
    uint8_t get()
    {
        return (arr_to_num(get_isreg()));
    }
};
typedef tmp_memory tmem;
int (main(void) )
{
    DDRB = B11110111;
    //DDRD = B11111110;
    //DDRD = DDRD | B11111100;
    tmem loop_count;
    loop_count = 255;    
    for (;;)
    {
        //loop_count = lcount;
        //PORTD = B00000000;
        //delay_(loop_count.get());
        //PORTD = B10000000;
        //delay_(loop_count.get());
    
        if (loop_count.get() == 0)
            loop_count = 255;
        else
            loop_count -= 1;
    }

    return 0;
}
