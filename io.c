# include "micro.h"
void io_set_direct(mdl_u8_t __dir, mdl_u8_t __pid) {
# ifdef __atmega328p
	if (__pid >= 0 && __pid <= 7)
	{
		if (__dir == io_direct_out)
			DDRD |= 1<<__pid;
		else if (__dir == io_direct_in)
			DDRD &= ~(1<<__pid);
	}
	else if (__pid > 7 && __pid <= 13)
	{
		if (__dir == io_direct_out)
			DDRB |= 1<<(__pid-8);
		else if (__dir == io_direct_in)
			DDRB &= ~(1<<(__pid-8));
	}
# endif
# ifdef __attiny85
	if (__pid >= 0 && __pid <= 4)
	{
		if (__dir == io_direct_out)
			DDRB |= 1<<__pid;
		else if (__dir == io_direct_in)
			DDRB &= ~(1<<__pid);
	}
# endif
}

void io_set_val(mdl_u8_t __val, mdl_u8_t __pid) {
# ifdef __atmega328p
	if (__pid >= 0 && __pid <= 7)
	{
		if (__val == io_val_high)
			PORTD |= 1<<__pid;
		else if (__val == io_val_low)
			PORTD &= ~(1<<__pid);
	}
	else if (__pid > 7 && __pid <= 13)
	{
		if (__val == io_val_high)
			PORTB |= 1<<(__pid-8);
		else if (__val == io_val_low)
			PORTB &= ~(1<<(__pid-8));
	}
# endif
# ifdef __attiny85
	if (__pid >= 0 && __pid <= 4)
	{
		if (__val == io_val_high)
			PORTB |= 1<<__pid;
		else if (__val == io_val_low)
			PORTB &= ~(1<<__pid);
	}
# endif
}

mdl_u8_t io_get_val(mdl_u8_t __pid) {
# ifdef __atmega328p
	if (__pid >= 0 && __pid <= 7)
		return (PIND>>__pid)&1;
	else if (__pid > 7 && __pid <= 13)
		return (PINB>>(__pid-8))&1;
# endif
# ifdef __attiny85
	if (__pid >= 0 && __pid <= 4)
		return (PINB>>__pid)&1;
# endif
	return 0;
}
