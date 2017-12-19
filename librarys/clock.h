
# include <avr/interrupt.h>
mdl_uint_t clock_us = 0;
void clock_init() {
	cli();
	TCCR0A = 1 << WGM01;
	OCR0A = 16;
	TIMSK0 = 1 << OCIE0A;

	TCCR0B = (1 << CS01)|(1 << CS00);
	sei();
}

mdl_uint_t get_clock_us() {return clock_us;}
ISR(TIMER0_COMPA_vect) {
    if (clock_us > (mdl_uint_t)~0)
		clock_us = 0;
	else
		clock_us+=64;
}
