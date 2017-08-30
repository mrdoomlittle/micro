# include "uart.h"
void uart_init(void) {
	UBRR0H = (mdl_u8_t)(BAUD_PRESCALE >> 8);
	UBRR0L = (mdl_u8_t)BAUD_PRESCALE;

	UCSR0C = (3 << UCSZ00);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_send_byte(mdl_u8_t __byte) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = __byte;
}

void uart_recv_byte(mdl_u8_t *__byte) {
	while (!(UCSR0A & (1 << RXC0)));
	*__byte = UDR0;
}

void uart_send_w16(mdl_u16_t __data) {
	uart_send_byte(__data & 0xFF);
	uart_send_byte(__data >> 8);
}

void uart_recv_w16(mdl_u16_t *__data) {
	mdl_u8_t byte = 0;
	uart_recv_byte(&byte);
	*__data |= byte;

	byte = 0;
	uart_recv_byte(&byte);
	*__data |= byte << 8;
}

void _uart_print(char *__s) {
	for (;*__s != '\0'; __s++) {
		uart_send_byte(*__s);
	}
}

void uart_print(char *__s) {
	_uart_print(__s);
	uart_send_byte('\0');
}

void uart_println(char *__s) {
	_uart_print(__s);
	uart_send_byte('\n');
}
