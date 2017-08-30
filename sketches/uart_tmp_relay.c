void micro_init(){}

void micro_tick() {
	mdl_u8_t inbound_byte = 0x0;
	uart_recv_byte(&inbound_byte);
	if (tmp_send_byte(&_tmp_io, inbound_byte) != TMP_SUCCESS) return;
	if (tmp_recv_byte(&_tmp_io, &inbound_byte) != TMP_SUCCESS) return;
	uart_send_byte(inbound_byte);
}
