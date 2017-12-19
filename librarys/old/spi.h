# include <mdlint.h>
# include "../micro.h"
# include <avr/interrupt.h>
mdl_u8_t spi_sck_pid = 0;
mdl_u8_t spi_mosi_pid = 0;
mdl_u8_t spi_moso_pid = 0;
# define NO_DELAY
# ifndef NO_DELAY
#	define DL 0.027
# endif
void spi_init(mdl_u8_t __sck_pid, mdl_u8_t __mosi_pid, mdl_u8_t __moso_pid) {
	spi_sck_pid = __sck_pid;
	spi_mosi_pid = __mosi_pid;
	spi_moso_pid = __moso_pid;
	set_pin_mode(DIGITAL_OUTPUT, __sck_pid);
	set_pin_mode(DIGITAL_INPUT, __mosi_pid);
	set_pin_mode(DIGITAL_OUTPUT, __moso_pid);
	set_pin_state(DIGITAL_LOW, __sck_pid);
}

void spi_send_bit(mdl_u8_t __bit) {
	set_pin_state(__bit&0x1, spi_moso_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	set_pin_state(DIGITAL_HIGH, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	set_pin_state(DIGITAL_LOW, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
}

void spi_recv_bit(mdl_u8_t *__bit_p) {
	set_pin_state(DIGITAL_HIGH, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	*__bit_p = get_pin_state(spi_mosi_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	set_pin_state(DIGITAL_LOW, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
}

void spi_send_w2(mdl_u8_t __w2) {
	spi_send_bit(__w2>>1);
	spi_send_bit(__w2);
}

void spi_recv_w2(mdl_u8_t *__w2) {
	mdl_u8_t bit;

	bit = 0;
	spi_recv_bit(&bit);

	(*__w2) |= bit<<1;

	bit = 0;
	spi_recv_bit(&bit);
	(*__w2) |= bit;
}

void spi_send_w4(mdl_u8_t __w4) {
	spi_send_w2(__w4>>2);
	spi_send_w2(__w4);
}

void spi_recv_w4(mdl_u8_t *__w4) {
	mdl_u8_t w4_part;

	w4_part = 0;
	spi_recv_w2(&w4_part);

	(*__w4) |= w4_part<<2;

	w4_part = 0;
	spi_recv_w2(&w4_part);
	(*__w4) |= w4_part;
}

void spi_send_w8(mdl_u8_t __w8) {
	spi_send_w4(__w8>>4);
	spi_send_w4(__w8);
}

void spi_recv_w8(mdl_u8_t *__w8) {
	mdl_u8_t w8_part;

	w8_part = 0;
	spi_recv_w4(&w8_part);

	(*__w8) |= w8_part<<4;

	w8_part = 0;
	spi_recv_w4(&w8_part);
	(*__w8) |= w8_part;
}
