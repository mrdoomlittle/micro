void micro_init(){}

void micro_tick() {
	mdl_u8_t outbound_byte = 'D';
	uart_send_byte(outbound_byte);
	if (tmp_send_byte(&_tmp_io, outbound_byte) != TMP_SUCCESS) return;
}

