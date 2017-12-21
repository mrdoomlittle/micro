# include <mdlint.h>
# include "../micro.h"
mdl_u8_t spi_sck_pid = 0;
mdl_u8_t spi_mosi_pid = 0;
mdl_u8_t spi_moso_pid = 0;
//# define NO_DELAY
# ifndef NO_DELAY
#	define DL 7000
# endif
void spi_init(mdl_u8_t __sck_pid, mdl_u8_t __mosi_pid, mdl_u8_t __moso_pid) {
	spi_sck_pid = __sck_pid;
	spi_mosi_pid = __mosi_pid;
	spi_moso_pid = __moso_pid;
	io_set_direct(io_direct_out, __sck_pid);
	io_set_direct(io_direct_in, __mosi_pid);
	io_set_direct(io_direct_out, __moso_pid);
	io_set_val(io_val_low, __sck_pid);
}

void spi_send_bit(mdl_u8_t __bit) {
	io_set_val(__bit&0x1, spi_moso_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	io_set_val(io_val_high, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	io_set_val(io_val_low, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
}

void spi_recv_bit(mdl_u8_t *__bit_p) {
	io_set_val(io_val_high, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	*__bit_p = io_get_val(spi_mosi_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
	io_set_val(io_val_low, spi_sck_pid);
# ifndef NO_DELAY
	_delay_us(DL);
# endif
}

void spi_send_2l(mdl_u8_t __2l) {
	spi_send_bit(__2l>>1);
	spi_send_bit(__2l);
}

void spi_recv_2l(mdl_u8_t *__2l) {
	mdl_u8_t bit;

	bit = 0;
	spi_recv_bit(&bit);

	(*__2l) |= bit<<1;

	bit = 0;
	spi_recv_bit(&bit);
	(*__2l) |= bit;
}

void spi_send_4l(mdl_u8_t __val) {
	spi_send_2l(__val>>2);
	spi_send_2l(__val);
}

void spi_recv_4l(mdl_u8_t *__val) {
	mdl_u8_t part;

	part = 0;
	spi_recv_2l(&part);
	*__val |= part<<2;

	part = 0;
	spi_recv_2l(&part);
	*__val |= part;
}

void spi_send_8l(mdl_u8_t __val) {
	spi_send_4l(__val>>4);
	spi_send_4l(__val);
}

void spi_recv_8l(mdl_u8_t *__val) {
	mdl_u8_t part;

	part = 0;
	spi_recv_4l(&part);
	*__val |= part<<4;

	part = 0;
	spi_recv_4l(&part);
	*__val |= part;
}
