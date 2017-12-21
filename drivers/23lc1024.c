# include "../librarys/spi.c"
mdl_u8_t _23lc1024_cs_pid = 0;
void _23lc1024_init(mdl_u8_t __sck_pid, mdl_u8_t __mosi_pid, mdl_u8_t __moso_pid, mdl_u8_t __cs_pid) {
	io_set_direct(io_direct_out, __cs_pid);
	spi_init(__sck_pid, __mosi_pid, __moso_pid);
	io_set_val(io_val_low, __cs_pid);
	spi_send_8l(0b00000001);
	spi_send_8l(0b00000000);
	io_set_val(io_val_high, __cs_pid);
	_23lc1024_cs_pid = __cs_pid;
}

void _23lc1024_send_addr(mdl_u32_t __addr) {
	spi_send_8l(__addr>>16&0xFF);
	spi_send_8l(__addr>>8&0xFF);
	spi_send_8l(__addr&0xFF);
}

void _23lc1024_put(mdl_u32_t __addr, mdl_u8_t __byte) {
	io_set_val(io_val_low, _23lc1024_cs_pid);
	spi_send_8l(0b00000010);
	_23lc1024_send_addr(__addr);
	spi_send_8l(__byte);
	io_set_val(io_val_high, _23lc1024_cs_pid);
# ifdef DL
	_delay_us(DL);
# endif
}

void _23lc1024_get(mdl_u32_t __addr, mdl_u8_t *__byte_p) {
	io_set_val(io_val_low, _23lc1024_cs_pid);
	spi_send_8l(0b00000011);
	_23lc1024_send_addr(__addr);
	spi_recv_8l(__byte_p);
	io_set_val(io_val_high, _23lc1024_cs_pid);
# ifdef DL
	_delay_us(DL);
# endif
}

