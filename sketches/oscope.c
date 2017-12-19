# include <avr/interrupt.h>
mdl_u8_t analog_pval = 0;
void micro_init() {
	ADCSRA |= 1 << ADEN;
	ADCSRA |= 1 << ADPS2;
	ADCSRA |= 1 << ADPS1;
	ADCSRA |= 1 << ADPS0;

	ADMUX |= 1 << ADLAR;
	ADCSRA |= 1 << ADIE;
	ADMUX |= 1 << REFS0;

	sei();

	ADCSRA |= 1 << ADSC;
}

void micro_tick() {
	uart_send_byte(analog_pval);
}

ISR(ADC_vect) {
	analog_pval = ADCH;
	ADCSRA |= 1 << ADSC;
}
