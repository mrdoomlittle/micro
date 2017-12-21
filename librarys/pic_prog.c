# include "pic_prog.h"
# define DELAY 0.1
void exit_prog_mode(struct pic_prog_t*);
void enter_prog_mode(struct pic_prog_t*);
void pic_prog_init(struct pic_prog_t *__pic_prog, mdl_u8_t __clk_pid, mdl_u8_t __data_pid, mdl_u8_t __pmg_pid, mdl_u8_t __mclr_pid) {
	io_set_direct(io_direct_out, __clk_pid);
	io_set_direct(io_direct_out, __data_pid);
	io_set_direct(io_direct_out, __pmg_pid);
	io_set_direct(io_direct_out, __mclr_pid);
	__pic_prog-> clk_pid = __clk_pid;
	__pic_prog-> data_pid = __data_pid;
	__pic_prog-> pgm_pid = __pmg_pid;
	__pic_prog-> mclr_pid = __mclr_pid;
	exit_prog_mode(__pic_prog);
	enter_prog_mode(__pic_prog);
}

void exit_prog_mode(struct pic_prog_t *__pic_prog) {
	io_set_val(io_val_low, __pic_prog->pgm_pid);
	io_set_val(io_val_low, __pic_prog->mclr_pid);
	_delay_ms(50);
}

void enter_prog_mode(struct pic_prog_t *__pic_prog) {
	io_set_val(io_val_high, __pic_prog->pgm_pid);
	io_set_val(io_val_high, __pic_prog->mclr_pid);
	_delay_ms(50);
}

mdl_u8_t ld_fdm_cmd = 0b00000011;
mdl_u8_t rd_fdm_cmd = 0b00000101;
mdl_u8_t ld_conf_cmd = 0b00000000;
mdl_u8_t begin_epc_cmd = 0b00001000;
mdl_u8_t begin_poc_cmd = 0b00011000;

mdl_u8_t ld_fpm_cmd = 0b00000010;
mdl_u8_t rd_fpm_cmd = 0b00000100;
mdl_u8_t bulk_er_s1_cmd = 0b00000001;
mdl_u8_t bulk_er_s2_cmd = 0b00000111;
mdl_u8_t bulk_er_pm_cmd = 0b00001001;
mdl_u8_t bulk_er_dm_cmd = 0b00001011;
mdl_u8_t incr_addr_cmd = 0b00000110;
mdl_u8_t end_prog_cmd = 0b00010111;

void pic_prog_send_cmd(struct pic_prog_t *__pic_prog, mdl_u8_t __cmd) {
	pic_prog_send(__pic_prog, __cmd, 6);
	_delay_ms(DELAY);
}

void pic_prog_read_data(struct pic_prog_t *__pic_prog, mdl_u16_t *__data) {
	mdl_u8_t p1_byte = 0x0, p2_byte = 0x0;
	mdl_u8_t null_bit = 0;

	pic_prog_read_bit(__pic_prog, &null_bit);
	_delay_ms(DELAY);
	pic_prog_recv(__pic_prog, &p1_byte, 8);
	pic_prog_recv(__pic_prog, &p2_byte, 6);
	pic_prog_read_bit(__pic_prog, &null_bit);
	_delay_ms(DELAY);

	*__data |= p1_byte;
	*__data |= p2_byte << 8;
}

void pic_prog_load_data(struct pic_prog_t *__pic_prog, mdl_u16_t __data) {
	mdl_u8_t p1_byte = 0x0, p2_byte = 0x0;

	p1_byte |= __data & 0xFF;
	p2_byte |= __data >> 8;

	pic_prog_write_bit(__pic_prog, 0);
	_delay_ms(DELAY);
	pic_prog_send(__pic_prog, p1_byte, 8);
	pic_prog_send(__pic_prog, p2_byte, 6);
	pic_prog_write_bit(__pic_prog, 0);
	_delay_ms(DELAY);
}

void reset(struct pic_prog_t *__pic_prog) {
	exit_prog_mode(__pic_prog);
	enter_prog_mode(__pic_prog);
}

void pic_prog_bulk_erase(struct pic_prog_t *__pic_prog) {
	reset(__pic_prog);

	pic_prog_send_cmd(__pic_prog, ld_fpm_cmd);
	pic_prog_load_data(__pic_prog, 0x3FFF);

	pic_prog_send_cmd(__pic_prog, bulk_er_pm_cmd);
	pic_prog_send_cmd(__pic_prog, bulk_er_dm_cmd);
	pic_prog_send_cmd(__pic_prog, begin_epc_cmd);
	_delay_ms(20);
	pic_prog_send_cmd(__pic_prog, bulk_er_pm_cmd);
	pic_prog_send_cmd(__pic_prog, bulk_er_dm_cmd);
	_delay_ms(DELAY);

	reset(__pic_prog);

	pic_prog_send_cmd(__pic_prog, ld_fdm_cmd);
	pic_prog_load_data(__pic_prog, 0x3FFF);

	pic_prog_send_cmd(__pic_prog, bulk_er_pm_cmd);
	pic_prog_send_cmd(__pic_prog, bulk_er_dm_cmd);
	_delay_ms(20);
	pic_prog_send_cmd(__pic_prog, bulk_er_pm_cmd);
	pic_prog_send_cmd(__pic_prog, bulk_er_dm_cmd);
	_delay_ms(DELAY);
}

void pic_prog_tick(struct pic_prog_t *__pic_prog) {
	mdl_u8_t i = 0x0;
	uart_recv_byte(&i);

	if (!i) {
		uart_send_byte(1);
		return;
	}

	switch(i) {
		case 0x1
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, ld_fpm_cmd);
		break;

		case 0x2:
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, rd_fpm_cmd);
		break;

		case 0x3:
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, incr_addr_cmd);
		break;

		case 0x4:
			uart_send_byte(0);
			reset(__pic_prog);

		break;
		case 0x5:
			uart_send_byte(0);
			pic_prog_bulk_erase(__pic_prog);
		break;

		case 0x7: {
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, ld_conf_cmd);
		break;

		case 0x8:
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, end_prog_cmd);
		break;

		case 0x9: {
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, ld_fdm_cmd);
		break;

		case 0xA: {
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, rd_fdm_cmd);
		break;

		case 0xB: {
			uart_send_byte(0);
			mdl_u16_t data = 0x0;
			uart_recv_16l(&data);
			pic_prog_load_data(__pic_prog, data);
			break;
		}

		case 0xC: {
			uart_send_byte(0);
			mdl_u16_t data = 0x0;
			pic_prog_read_data(__pic_prog, &data);
			uart_send_16l(data);
			break;
		}

		case 0xD:
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, begin_poc_cmd);
		break;
		case 0xE:
			uart_send_byte(0);
			pic_prog_send_cmd(__pic_prog, begin_epc_cmd);
		break;

		default:
			uart_send_byte(1);
		return;
	}
}

void pic_prog_send(struct pic_prog_t *__pic_prog, mdl_u8_t __byte, mdl_uint_t __n) {
	for (mdl_u8_t point = 0; point != __n; point ++) {
		pic_prog_write_bit(__pic_prog, (__byte >> point) & 1);
		_delay_ms(DELAY);
	}
}

void pic_prog_recv(struct pic_prog_t *__pic_prog, mdl_u8_t *__byte, mdl_uint_t __n) {
	mdl_u8_t off = 0;
	for (;off != __n;off++) {
		mdl_u8_t __bit = 0x0;
		pic_prog_read_bit(__pic_prog, &__bit);
		*__byte |= __bit<<off;
		_delay_ms(DELAY);
	}
}

void pic_prog_write_bit(struct pic_prog_t *__pic_prog, mdl_u8_t __bit) {
	io_set_direct(io_direct_out, __pic_prog-> data_pid);
	io_set_val(io_val_high, __pic_prog-> clk_pid);
	io_set_val(__bit, __pic_prog->data_pid);
	_delay_ms(DELAY);
	io_set_val(io_val_low, __pic_prog-> clk_pid);
	_delay_ms(DELAY);
}

void pic_prog_read_bit(struct pic_prog_t *__pic_prog, mdl_u8_t *__bit) {
	io_set_direct(io_direct_in, __pic_prog-> data_pid);
	io_set_val(io_val_high, __pic_prog-> clk_pid);
	_delay_ms(DELAY);

	*__bit = io_get_val(__pic_prog-> data_pid);

	io_set_val(io_val_low, __pic_prog-> clk_pid);
	_delay_ms(DELAY);
}
