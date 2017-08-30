# include <eint_t.h>
# include "../micro.h"
# include <avr/interrupt.h>
mdl_u8_t spi_sck_pid = 0;
mdl_u8_t spi_moso_pid = 0;
void spi_init(mdl_u8_t __spi_sck_pid, mdl_u8_t __spi_mosi_pid, mdl_u8_t __spi_moso_pid) {
	spi_sck_pid = __spi_sck_pid;
	spi_moso_pid = __spi_moso_pid;
	set_pin_mode(DIGITAL_OUTPUT, __spi_sck_pid);
	set_pin_mode(DIGITAL_OUTPUT, __spi_moso_pid);
	set_pin_state(DIGITAL_LOW, __spi_sck_pid);
}

void spi_send_bit(mdl_u8_t __bit) {
	set_pin_state(__bit & 1, spi_moso_pid);
	_delay_us(20);
	set_pin_state(DIGITAL_HIGH, spi_sck_pid);
	_delay_us(20);
	set_pin_state(DIGITAL_LOW, spi_sck_pid);
	_delay_us(20);
}

void spi_send_w2(mdl_u8_t __w2) {
	spi_send_bit(__w2 >> 1);
	spi_send_bit(__w2);
}

void spi_send_w4(mdl_u8_t __w4) {
	spi_send_w2(__w4 >> 2);
	spi_send_w2(__w4);
}

void spi_send_w8( mdl_u8_t __w8) {
	spi_send_w4(__w8 >> 4);
	spi_send_w4(__w8);
}
