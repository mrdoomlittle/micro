
# include <avr/interrupt.h>
mdl_uint_t clock_ms = 0;
void clock_init() {
	TCCR0A = 1 << WGM01;
	OCR0A = 16;
	TIMSK0 = 1 << OCIE0A;

	sei();

	TCCR0B = (1 << CS02) | (1 << CS00);
}

mdl_uint_t get_clock_ms() {return clock_ms;}

ISR(TIMER0_COMPA_vect) {
    if (clock_ms == (mdl_uint_t)~(mdl_uint_t)0)
		clock_ms = 0;
	else
		clock_ms++;
}
