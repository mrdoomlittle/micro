# include "../librarys/spi.c"
# define SCK 2
# define MOSI 3
# define MOSO 4
# define RESET 5
# define RST_HIGH DIGITAL_LOW
# define RST_LOW DIGITAL_HIGH
void send_instr(mdl_u32_t, mdl_u8_t*, mdl_u8_t*);
mdl_u32_t mk_instr(mdl_u8_t, mdl_u8_t, mdl_u8_t, mdl_u8_t);

void micro_init() {
	set_pin_mode(DIGITAL_OUT, 6);
	set_pin_state(DIGITAL_HIGH, 6);

	set_pin_mode(DIGITAL_OUT, RESET);
	set_pin_state(RST_LOW, RESET);

	spi_init(SCK, MOSI, MOSO);
	set_pin_state(DIGITAL_LOW, SCK);
	_delay_ms(30);

	set_pin_state(RST_HIGH, RESET);
	_delay_ms(200);
/*
	set_pin_state(RST_HIGH, RESET);
	_delay_ms(2);
	set_pin_state(RST_LOW, RESET);
	_delay_ms(2);

	_delay_ms(30);

	mdl_u8_t reply;

	_again:
	reply = 0;
	send_instr(mk_instr(0xAC, 0x53, 0x0, 0x0), &reply, NULL);
	if (reply != 0x53) {
		set_pin_state(RST_LOW, RESET);
		_delay_ms(2);
		set_pin_state(RST_HIGH, RESET);
		_delay_ms(20);
		goto _again;
	}
*/
	set_pin_state(DIGITAL_LOW, 6);
	_delay_ms(8);
}

void send_instr(mdl_u32_t __i, mdl_u8_t *__b3, mdl_u8_t *__b4) {
	spi_send_w8(__i>>24);
	spi_send_w8(__i>>16);

	if (__b3 != NULL)
		spi_recv_w8(__b3);
	else
		spi_send_w8(__i>>8);

	if (__b4 != NULL)
		spi_recv_w8(__b4);
	else
		spi_send_w8(__i);
}

mdl_u32_t mk_instr(mdl_u8_t __b1, mdl_u8_t __b2, mdl_u8_t __b3, mdl_u8_t __b4) {
	return ((mdl_u32_t)__b1<<24)|((mdl_u32_t)__b2<<16)|((mdl_u32_t)__b3<<8)|__b4;
}

void micro_tick() {
	mdl_u8_t cmd = 0;
	uart_recv_byte(&cmd);
	uart_send_byte(cmd);
	switch(cmd) {
		case 0x1: {
			uart_send_byte(1);
			mdl_u16_t addr = 0;
			uart_recv_w16(&addr);

			mdl_u8_t byte = 0;
			send_instr(mk_instr(0xA0, addr>>8, addr, 0x0), NULL, &byte);
			uart_send_byte(byte);
			break;
		}
		case 0x2: {
			uart_send_byte(1);
			mdl_u16_t addr = 0;
			uart_recv_w16(&addr);

			mdl_u8_t byte = 0;
			uart_recv_byte(&byte);
			send_instr(mk_instr(0xC0, addr>>8, addr, byte), NULL, NULL);
			break;
		}
		case 0x3: {
			uart_send_byte(1);
			send_instr(mk_instr(0xAC, 0x80, 0x0, 0x0), NULL, NULL);
			break;
		}
		case 0x4: {
			uart_send_byte(1);
			mdl_u8_t poll = 0;
			send_instr(mk_instr(0xF0, 0x0, 0x0, 0x0), NULL, &poll);
			uart_send_byte(poll);
			break;
		}
		case 0x5: {
			uart_send_byte(1);
			mdl_u16_t addr = 0, data = 0;

			uart_recv_w16(&addr);
			uart_recv_w16(&data);

			send_instr(mk_instr(0x40, addr>>8, addr, data>>8), NULL, NULL);
			send_instr(mk_instr(0x48, addr>>8, addr, data), NULL, NULL);
			break;
		}
		case 0x6: {
			uart_send_byte(1);
			mdl_u16_t addr = 0, data = 0;
			uart_recv_w16(&addr);

			mdl_u8_t part = 0;
			send_instr(mk_instr(0x20, addr>>8, addr, 0x0), NULL, &part);
			data |= part<<8;

			part = 0;
			send_instr(mk_instr(0x28, addr>>8, addr, 0x0), NULL, &part);
			data |= part;

			uart_send_w16(data);
			break;
		}
		case 0x7: {
			uart_send_byte(1);
			mdl_u16_t addr = 0;
			uart_recv_w16(&addr);
			send_instr(mk_instr(0x4C, addr>>8, addr, 0x0), NULL, NULL);
			break;
		}
		case 0x8: {
			uart_send_byte(1);
			mdl_u8_t fuse_byte = 0;
			send_instr(mk_instr(0x50, 0x0, 0x0, 0x0), NULL, &fuse_byte);
			uart_send_byte(fuse_byte);
			break;
		}
		case 0x9: {
			uart_send_byte(1);
			mdl_u8_t fuse_byte = 0;
			uart_recv_byte(&fuse_byte);
			send_instr(mk_instr(0xAC, 0xA0, 0x0, fuse_byte), NULL, NULL);
			break;
		}
		case 0xA: {
			uart_send_byte(1);
			mdl_u8_t high_fuse_byte = 0;
			send_instr(mk_instr(0x58, 0x08, 0x0, 0x0), NULL, &high_fuse_byte);
			uart_send_byte(high_fuse_byte);
			break;
		}
		case 0xB: {
			uart_send_byte(1);
			mdl_u8_t high_fuse_byte = 0;
			uart_recv_byte(&high_fuse_byte);
			send_instr(mk_instr(0xAC, 0xA8, 0x0, high_fuse_byte), NULL, NULL);
			break;
		}
		default:
			uart_send_byte(0);
	}
}
