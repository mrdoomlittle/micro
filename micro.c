# include "micro.h"
int main(void) {
	_delay_ms(3000);
	uart_init();
	micro_init();
# ifdef __WITH_TMP
	_tmp_init();
# endif
	for (;;) {
		micro_tick();
	}
}

void micro_init() {

}

void micro_tick() {

}

/*
# include "shift_reg.h"
# define _PD1 5
# define _PD2 0
# define _PD3 1
# define _PD4 2
# define _PD5 3
# define _PD6 4
# define _PD7 5
# define _PC2 6
# define RESET 13

struct shift_reg_t shift_reg = {
	.data_pid = 2,
	.latch_pid = 3,
	.clock_pid = 4,
	.data_flow = SHIFT_REG_OUT
};

void wait() {
	while(!get_pin_state(_PD1));
}

void micro_init() {
	set_pin_mode(DIGITAL_IN, _PD1);
	set_pin_mode(DIGITAL_OUT, _PD2);
	set_pin_mode(DIGITAL_OUT, _PD3);
	set_pin_mode(DIGITAL_OUT, _PD4);
	set_pin_mode(DIGITAL_OUT, _PD5);
	set_pin_mode(DIGITAL_OUT, _PD6);
	set_pin_mode(DIGITAL_OUT, _PD7);
	shift_reg_init(&shift_reg);
	shift_reg_set(&shift_reg, shift_reg.ps_list);
//	mdl_u8_t ps[8] = {1, 0, 1, 0, 1, 0, 1, 0};
//	shift_reg_set(&shift_reg, ps);
}

void micro_tick() {}
*/
/*
void micro_init() {
	set_pin_mode(DIGITAL_OUT, 2);
}

# define _1000HZ ((F_CPU/16/1000000)/2)

void delay_me(mdl_uint_t __hz) {
	mdl_uint_t i = 0;

	_again:
	if (i >= (300-__hz)) return;
	_delay_us(_1000HZ);
	i++;
	goto _again;
}

mdl_uint_t hz = 298;
void micro_tick() {
	set_pin_state(DIGITAL_HIGH, 2);
	delay_me(hz);
	set_pin_state(DIGITAL_LOW, 2);
	delay_me(hz);
}

//# include "sketches/at_prog.c"
*/
/*
# include "drivers/23lc1024.c"
void micro_init() {
	_23lc1024_init(2, 3, 4, 5);
}

void micro_tick() {
	mdl_u8_t static c = 0;
	cli();
	_23lc1024_put(0, c++);
	_23lc1024_put(1, 212);
	mdl_u8_t byte = 0;
	_delay_ms(2);
	_23lc1024_get(0, &byte);

	sei();

	_delay_ms(20);
	uart_send_byte(byte);
}
*/
//# include "sketches/uart_tmp_relay.c"
//# include "sketches/prog_uploader.c"
//# include "sketches/at_prog.c"
//# include "sketches/oscope.c"
//# include "sketches/tmp_client.c"
//# include "sketches/tmp_server.c"
/*
# include "librarys/spi.h"
void send(mdl_u16_t __data) {
	cli();
	mdl_u8_t addr = 0, command = 0;

	spi_send_w4(addr);
	spi_send_w2(command);
	spi_send_bit(0);
	spi_send_bit(__data);
	spi_send_w8(__data >> 1);
	sei();
}

void micro_init() {
	spi_init(2, 4, 3);
}

mdl_u8_t x = 0;
void micro_tick() {
	send(x);
	_delay_ms(100);
	x++;
}
*/
/*
# include "librarys/spi.h"
# include <avr/interrupt.h>
mdl_u8_t volatile analog_pval = 0;

double r1, r2;

double voltage_hold;

void micro_init() {
	r1 = 1.0*1000000.0;
	r2 = 150.0*1000.0;
	voltage_hold = (12.00*r2)/(r1+r2);

	ADCSRA |= 1 << ADEN;
	ADCSRA |= 1 << ADPS2;
	ADCSRA |= 1 << ADPS1;
	ADCSRA |= 1 << ADPS0;

	ADMUX |= 1 << ADLAR;
	ADCSRA |= 1 << ADIE;
	ADMUX |= 1 << REFS0;

	sei();

	ADCSRA |= 1 << ADSC;
	spi_init(2, 4, 3);
}

void send(mdl_u16_t __data) {
	cli();
	mdl_u8_t addr = 0, command = 0;

	spi_send_w4(addr);
	spi_send_w2(command);
	spi_send_bit(0);
	spi_send_bit(__data);
	spi_send_w8(__data >> 1);
	sei();
}

# define max_res 255
# define min_res 0
mdl_u16_t res = max_res;
void micro_tick() {
	double voltage = (double)analog_pval*(5.0/255.0);
	if (voltage < voltage_hold) {
		if (res < max_res) res++;
	} else if (voltage > voltage_hold) {
		if (res > min_res) res--;
	}

	send(res);
}

ISR(ADC_vect) {
	analog_pval = ADCH;
	ADCSRA |= 1 << ADSC;
}
*/
//# include "sketches/prog_uploader.h"
//# include <avr/eeprom.h>

//# include "librarys/pic_prog.h"
//struct pic_prog_t pic_prog;
//void micro_init() {
//	pic_prog_init(&pic_prog, 2, 3, 4, 5);
//}

//void micro_tick() {
//	pic_prog_tick(&pic_prog);
//}

/*
# include "modules/eeprom.h"
# include "uart.h"
struct eeprom_t eeprom;
struct _24lc256_t _24lc256;
void micro_init() {
	uart_init();
	_24lc256_init(&_24lc256, 2, 3);
	eeprom_init(&eeprom, &_24lc256);
}

void micro_tick() {
	mdl_u16_t my_data = 0;
//	uart_recv_byte(&inbound_byte);
//	_24lc256_write_byte(&_24lc256, inbound_byte, 0);


//	inbound_byte = 0;
	eeprom_get_w16(&eeprom, &my_data, 0);

	uart_send_byte(my_data);
	uart_send_byte(my_data >> 8);

	if (my_data > 21299) {
		eeprom_put_w16(&eeprom, 0, 0);
	} else {
		eeprom_put_w16(&eeprom, my_data + 1, 0);
	}

	eeprom_page_update(&eeprom, 0);
	//_delay_ms(200);

}
*/

