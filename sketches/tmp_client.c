void micro_init(){}

void micro_tick() {
	mdl_u8_t inbound_byte = 0x0;
	if (tmp_recv(&_tmp_io, tmp_io_buff(&inbound_byte, 1)) == TMP_SUCCESS) {
		uart_send_byte(inbound_byte);
	}

	uart_send_byte(inbound_byte);

}

