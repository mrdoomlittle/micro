# ifndef __wiring__digital__hpp__
# define __wiring__digital__hpp__
# define DIGITAL_HIGH 0x1
# define DIGITAL_LOW 0x0
# define DIGITAL_OUTPUT 0x1
# define DIGITAL_INPUT 0x0
# include <avr/io.h>
# include <util/delay.h>
# include "binary.hpp"
# include <stdint.h>

namespace qg 
{
    void set_pin_mode(uint8_t __pmode, uint8_t __pid)
    {
        if (__pid >= 0 && __pid <= 7)
        {
            if (__pmode == DIGITAL_OUTPUT)
                DDRD |= 1 << __pid;
            else if (__pmode == DIGITAL_INPUT)
                DDRD &= ~(1 << __pid);
        } 
        else if (__pid > 7 && __pid < 15)
        {
            if (__pmode == DIGITAL_OUTPUT)
                DDRB |= 1 << (__pid - 8);
            else if (__pmode == DIGITAL_INPUT)
                DDRB &= ~(1 << (__pid - 8));
        }
    }

    void set_pin_state(uint8_t __pstate, uint8_t __pid)
    {
        if (__pid >= 0 && __pid <= 7)
        {
            if (__pstate == DIGITAL_HIGH)
                PORTD |= 1 << __pid;
            else if (__pstate == DIGITAL_LOW)
                PORTD &= ~(1 << __pid);
        }
        else if (__pid > 7 && __pid < 15)
        {
            if (__pstate == DIGITAL_HIGH)
                PORTB |= 1 << (__pid - 8);
            else if (__pstate == DIGITAL_LOW)
                PORTB &= ~(1 << (__pid - 8));
        }   
    }

    uint8_t get_pin_state(uint8_t __pid)
    {
        if (__pid >= 0 && __pid <= 7)
        {
            uint8_t val = PIND;
            return (((val >> __pid) & 1) == 1? 1 : 0);
        }
        else if (__pid > 7 && __pid < 15)
        {
            uint8_t val = PINB;
            return (((val >> (__pid - 8)) & 1) == 1? 1 : 0);
        }
    }
}

# endif /*__wiring__digital__hpp__*/
