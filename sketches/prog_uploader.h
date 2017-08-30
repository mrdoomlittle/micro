# include "../modules/eeprom.h"
# include "../uart.h"
# include <bc_interp.h>
struct eeprom_t eeprom;
struct _24lc256_t _24lc256;

mdl_u16_t pc = 0;
mdl_u8_t get_byte() {
	mdl_u8_t byte = 0;
	eeprom_get_w8(&eeprom, &byte, pc);
	return byte;
}

void pc_incr() {
	pc++;
}

void set_pc(mdl_u16_t __pc) {
	pc = __pc;
}

mdl_u16_t get_pc() {
	return pc;
}

mdl_uint_t addr = 0;

struct bc_interp_t bc_interp = {
	.stack_size = 80,
	.heep_size = 21,
	.get_byte = &get_byte,
	.set_pc = &set_pc,
	.get_pc = &get_pc,
	.pc_incr = &pc_incr
};

void micro_init() {
	uart_init();
	_24lc256_init(&_24lc256, 2, 3);
	eeprom_init(&eeprom, &_24lc256);
	set_pin_mode(DIGITAL_OUTPUT, 7);
	bci_init(&bc_interp);
}

void micro_tick() {
	mdl_u8_t instruct = '\0';
	uart_recv_byte(&instruct);

	if (instruct == 'p') {
		set_pin_state(DIGITAL_HIGH, 7);
		mdl_u8_t recved_byte;
		uart_recv_byte(&recved_byte);
		eeprom_put_w8(&eeprom, recved_byte, addr);
		addr++;
	}

	if (instruct == 'e') {
		eeprom_page_update(&eeprom, addr >> 6);
	}

	if (instruct == 'c') {
		set_pin_state(DIGITAL_LOW, 7);
		mdl_u8_t byte = 0;
		eeprom_get_w8(&eeprom, &byte, addr);
		uart_send_byte(byte);
		addr++;
	}

	if (instruct == 'r') addr = 0;

	if (instruct == 'x') {
		pc = 0;
		bci_exec(&bc_interp);
	}
}
