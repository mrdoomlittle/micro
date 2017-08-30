# include "serial_comm.h"
void scomm_init(struct scomm_t *__scomm, mdl_u8_t __rx_pid, mdl_u8_t __tx_pid) {
/*
	set_pin_mode(DIGITAL_INPUT, __rx_pid);
	set_pin_mode(DIGITAL_OUTPUT, __tx_pid);
	set_pin_state(DIGITAL_HIGH, __tx_pid);
*/
	__scomm-> rx_pid = __rx_pid;
	__scomm-> tx_pid = __tx_pid;
}
#include <util/setbaud.h>
void scomm_send_byte(struct scomm_t *__scomm, mdl_u8_t __byte) {
//	while((UCSRA & UDRE) != UDRE) {}
//	UDR = __byte;
	
/*
	set_pin_state(DIGITAL_LOW, __scomm-> tx_pid);
	_delay_us(DELAY);

	for (uint8_t point = 0; point != 8; point ++) {
		set_pin_state((__byte >> point) & 1, __scomm-> tx_pid);
		_delay_us(DELAY);
	}

	_delay_us(DELAY);

	set_pin_state(DIGITAL_HIGH, __scomm-> tx_pid);
	_delay_us(DELAY);
*/
}

void scomm_recv_byte(struct scomm_t *__scomm, mdl_u8_t *__byte) {
/*
	while (get_pin_state(__scomm-> rx_pid) != DIGITAL_HIGH) {}
	_delay_us(DELAY);

	*__byte = 0x0;
	for (uint8_t point = 0; point != 8; point ++) {
		*__byte |= get_pin_state(__scomm-> rx_pid) << point;
		_delay_us(DELAY);
	}

	_delay_us(DELAY);

	_delay_us(DELAY);
*/
}
