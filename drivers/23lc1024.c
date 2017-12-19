# include "../librarys/spi.c"
mdl_u8_t _23lc1024_cs_pid = 0;
void _23lc1024_init(mdl_u8_t __sck_pid, mdl_u8_t __mosi_pid, mdl_u8_t __moso_pid, mdl_u8_t __cs_pid) {
	set_pin_mode(DIGITAL_OUTPUT, __cs_pid);
	spi_init(__sck_pid, __mosi_pid, __moso_pid);
	set_pin_state(DIGITAL_LOW, __cs_pid);
	spi_send_w8(0b00000001);
	spi_send_w8(0b00000000);
	set_pin_state(DIGITAL_HIGH, __cs_pid);
	_23lc1024_cs_pid = __cs_pid;
}

void _23lc1024_send_addr(mdl_u32_t __addr) {
	spi_send_w8(__addr>>16&0xFF);
	spi_send_w8(__addr>>8&0xFF);
	spi_send_w8(__addr&0xFF);
}

void _23lc1024_put(mdl_u32_t __addr, mdl_u8_t __byte) {
	set_pin_state(DIGITAL_LOW, _23lc1024_cs_pid);
	spi_send_w8(0b00000010);
	_23lc1024_send_addr(__addr);
	spi_send_w8(__byte);
	set_pin_state(DIGITAL_HIGH, _23lc1024_cs_pid);
# ifdef DL
	_delay_us(DL);
# endif
}

void _23lc1024_get(mdl_u32_t __addr, mdl_u8_t *__byte_p) {
	set_pin_state(DIGITAL_LOW, _23lc1024_cs_pid);
	spi_send_w8(0b00000011);
	_23lc1024_send_addr(__addr);
	spi_recv_w8(__byte_p);
	set_pin_state(DIGITAL_HIGH, _23lc1024_cs_pid);
# ifdef DL
	_delay_us(DL);
# endif
}

