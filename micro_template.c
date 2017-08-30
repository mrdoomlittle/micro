# include "micro.h"

int main(void) {
	_delay_ms(5000);
	micro_init();
	for (;;) {
		micro_tick();
	}
}

# include "uart.h"
void micro_init() {
	uart_init();
}

void micro_tick() {
	
}

