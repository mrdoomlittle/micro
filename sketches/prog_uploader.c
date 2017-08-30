# include "../modules/eeprom.h"
# include <bci.h>
struct eeprom_t eeprom;
struct _24lc256_t _24lc256;

bci_addr_t pc = 0;

mdl_u8_t get_byte() {
	mdl_u8_t byte = 0;
	eeprom_get_w8(&eeprom, &byte, pc);
	return byte;
}

void pc_incr() {
	pc++;
}

void set_pc(bci_addr_t __pc) {
	pc = __pc;
}

bci_addr_t get_pc() {
	return pc;
}

struct m_arg {
	mdl_u8_t pin_mode, pin_state, pid;
} __attribute__((packed));

void* test_func(mdl_u8_t __id, void *__arg) {
	mdl_u8_t static ret_val;
	struct m_arg *_m_arg = (struct m_arg*)__arg;

	switch(__id) {
		case 0:
			set_pin_mode(_m_arg->pin_mode, _m_arg->pid);
		break;
		case 1:
			set_pin_state(_m_arg->pin_state, _m_arg->pid);
		break;
		case 2:
			ret_val = 0;
			ret_val = get_pin_state(_m_arg->pid);
		break;
		case 3:
			for (mdl_uint_t mila = 0; mila != *(mdl_u16_t*)__arg; mila++) {_delay_ms(1);}
		break;
# ifdef __WITH_TMP
		case 4:
			tmp_send_byte(&_tmp_io, *(mdl_u8_t*)__arg);
		break;
		case 5:
			tmp_recv_byte(&_tmp_io, (mdl_u8_t*)__arg);
		break;
# endif 
		case 6:
			uart_send_byte(*(mdl_u8_t*)__arg);
		break;
	}

	return (void*)&ret_val;
}

struct bci _bci = {
	.stack_size = 120,
	.get_byte = &get_byte,
	.set_pc = &set_pc,
	.get_pc = &get_pc,
	.pc_incr = &pc_incr
};

void act_indc() {
	mdl_u8_t static state = 0;
	set_pin_state((state = ~state & 0x1), 4);
}

void iei(void *__arg) {
	act_indc();
}

void micro_init() {
	_24lc256_init(&_24lc256, 2, 3);
	eeprom_init(&eeprom, &_24lc256);

    bci_init(&_bci);
    bci_set_extern_fp(&_bci, &test_func);
	bci_set_act_indc_fp(&_bci, &act_indc);
	bci_set_iei_fp(&_bci, &iei);
	set_pin_mode(DIGITAL_OUTPUT, 4);
//	bci_exec(&_bci, 0x0, 0);
}

# define send_ack(__val) uart_send_byte(__val)
mdl_uint_t addr = 0;
void micro_tick() {
	mdl_u8_t i = 0x0;
	uart_recv_byte(&i);

	switch(i) {
		case 0x1: {
			send_ack(1);
			mdl_u8_t inbound_byte;
			uart_recv_byte(&inbound_byte);
			eeprom_put_w8(&eeprom, inbound_byte, addr);
			addr++;
			break;
		}
		case 0x2: {
			send_ack(1);
			mdl_u8_t outbound_byte = 0;
			eeprom_get_w8(&eeprom, &outbound_byte, addr);
			uart_send_byte(outbound_byte);
			addr++;
			break;
		}

		case 0x3:
			send_ack(1);
			eeprom_page_update(&eeprom, addr >> 6);
		break;

		case 0x4:
			send_ack(1);
			addr = 0x0;
		break;

		case 0x5: {
			send_ack(1);
			bci_exec(&_bci, 0x0, 0);
			set_pin_state(DIGITAL_LOW, 4);
			break;
		}
	}
}
