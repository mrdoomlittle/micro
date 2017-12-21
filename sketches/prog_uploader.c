# define USE_EEPROM
# ifdef USE_EEPROM
#	include "../modules/eeprom.h"
# else
#	include "../drivers/23lc1024.c"
# endif
# include <mdl/bci.h>
# ifdef USE_EEPROM
struct eeprom eeprom;
struct _24lc256 _24lc256;
# endif
bci_addr_t ip = 0;

mdl_u8_t get_byte(bci_off_t __off) {
	mdl_u8_t byte = 0;
# ifdef USE_EEPROM
	eeprom_get_8l(&eeprom, &byte, ip+__off);
# else
	_23lc1024_get(ip+__off, &byte);
# endif
	return byte;
}

void ip_incr(bci_uint_t __by) {
	ip+=__by;
}

void set_ip(bci_addr_t __ip) {
	ip = __ip;
}

bci_addr_t get_ip() {
	return ip;
}

struct m_arg {
	mdl_u8_t direct, val, pid;
} __attribute__((packed));

void* extern_call(mdl_u8_t __id, void *__arg_p) {
	mdl_u64_t static ret_val;
	struct m_arg *arg = (struct m_arg*)__arg_p;

	switch(__id) {
		case 0:
			io_set_direct(arg->direct, arg->pid);
		break;
		case 1:
			io_set_val(arg->val, arg->pid);
		break;
		case 2:
			ret_val = io_get_val(arg->pid);
		break;
		case 3: {
			mdl_uint_t milli_sec = 0;
			_another:
			if (milli_sec++ == *(mdl_u16_t*)__arg_p) break;
			_delay_ms(1);
			goto _another;
		}
		break;
# ifdef __mdl_tmp
		case 4:
			tmp_send_byte(&_tmp_io, *(mdl_u8_t*)__arg_p);
		break;
		case 5:
			tmp_recv_byte(&_tmp_io, (mdl_u8_t*)__arg_p);
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
	.stack_size = 100,
	.get_byte = &get_byte,
	.set_ip = &set_ip,
	.get_ip = &get_ip,
	.ip_incr = &ip_incr,
	.prog_size = 0xFFFF // stop it from causing issue
};

void act_indc() {
	mdl_u8_t static state = 0;
	state = ~state;
	io_set_direct(io_direct_out, 13);
	io_set_val(state&0x1, 13);
}

void iei(void *__arg) {
	act_indc();
}

void micro_init() {
# ifdef USE_EEPROM
	_24lc256_init(&_24lc256, 2, 3);
	eeprom_init(&eeprom, &_24lc256);
# else
	_23lc1024_init(2, 3, 4, 5);
# endif
	bci_init(&_bci, 0);
    bci_set_exc(&_bci, &extern_call);
	bci_set_iei(&_bci, &iei);
	io_set_direct(io_direct_out, 11);
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
			eeprom_put_8l(&eeprom, inbound_byte, addr);
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
			eeprom_get_8l(&eeprom, &outbound_byte, addr);
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
			eeprom_page_update(&eeprom, (addr-1)>>6);
# endif
		break;

		case 0x4:
			send_ack(1);
			addr = 0x0;
		break;

		case 0x5: {
			send_ack(1);
			cli();
			if (bci_exec(&_bci, 0x0000, NULL, NULL, 0) == BCI_SUCCESS) {
				io_set_val(io_val_high, 11);
			} else
				io_set_val(io_val_low, 11);
			sei();
			send_ack(1);
			break;
		}
	}
}
