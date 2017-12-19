//# define USE_EEPROM
# ifdef USE_EEPROM
#	include "../modules/eeprom.h"
# else
#	include "../drivers/23lc1024.c"
# endif
# include <bci.h>
# ifdef USE_EEPROM
struct eeprom_t eeprom;
struct _24lc256_t _24lc256;
# endif
bci_addr_t pc = 0;

mdl_u8_t get_byte() {
	mdl_u8_t byte = 0;
# ifdef USE_EEPROM
	eeprom_get_w8(&eeprom, &byte, pc);
# else
	_23lc1024_get(pc, &byte);
# endif
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
	mdl_u64_t static ret_val;
	struct m_arg *_m_arg = (struct m_arg*)__arg;

	switch(__id) {
		case 0:
			set_pin_mode(_m_arg->pin_mode, _m_arg->pid);
		break;
		case 1:
			set_pin_state(_m_arg->pin_state, _m_arg->pid);
		break;
		case 2:
			ret_val = get_pin_state(_m_arg->pid);
		break;
		case 3: {
			mdl_uint_t milli_sec = 0;
			_another:
			if (milli_sec++ == *(mdl_u16_t*)__arg) break;
			_delay_ms(1);
			goto _another;
		}
		break;
# ifdef __WITH_TMP
		case 4:
			tmp_send_byte(&_tmp_io, *(mdl_u8_t*)__arg);
		break;
		case 5:
			tmp_recv_byte(&_tmp_io, (mdl_u8_t*)__arg);
		break;
		case 6:
			ret_val = tmp_par_arcv_sig(&_tmp_io);
		break;
		case 7:
			ret_val = tmp_par_asnd_sig(&_tmp_io);
		break;
# endif
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
}

void iei(void *__arg) {
}

void micro_init() {
# ifdef USE_EEPROM
	_24lc256_init(&_24lc256, 2, 3);
	eeprom_init(&eeprom, &_24lc256);
# else
	_23lc1024_init(2, 3, 4, 5);
# endif
    bci_init(&_bci);
    bci_set_extern_fp(&_bci, &test_func);
	bci_set_act_indc_fp(&_bci, &act_indc);
	bci_set_iei_fp(&_bci, &iei);
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
# ifdef USE_EEPROM
			eeprom_put_w8(&eeprom, inbound_byte, addr);
# else
			cli();
			_23lc1024_put(addr, inbound_byte);
			sei();
# endif
			addr++;
			break;
		}
		case 0x2: {
			send_ack(1);
			mdl_u8_t outbound_byte = 0;
# ifdef USE_EEPROM
			eeprom_get_w8(&eeprom, &outbound_byte, addr);
# else
			cli();
			_23lc1024_get(addr, &outbound_byte);
			sei();
# endif
			uart_send_byte(outbound_byte);
			addr++;
			break;
		}

		case 0x3:
			send_ack(1);
# ifdef USE_EEPROM
			eeprom_page_update(&eeprom, addr>>6);
# endif
		break;

		case 0x4:
			send_ack(1);
			addr = 0x0;
		break;

		case 0x5: {
			send_ack(1);
			cli();
			bci_exec(&_bci, 0x0, 0);
			sei();
			send_ack(1);
			break;
		}
	}
}
