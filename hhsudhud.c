# include "micro.h"
# include "serial_comm.h"
int main(void) {
	_delay_ms(5000);
		micro_init();
	for (;;) {
		micro_tick();
	}
}

struct scomm_t scomm;
# define A0_PID 5
# define A1_PID 6
# define A2_PID 7
# define CLK_PID 8
# define DAT_PID 9
# define ERR_PID 10
# define DD 10
void write(mdl_u8_t __bstate) {
//	scomm_print(&scomm, __bstate? "WRITE: HIGH\n" : "WRITE: LOW\n");
	set_pin_mode(DIGITAL_OUTPUT, DAT_PID);
	set_pin_state(__bstate, DAT_PID);
	_delay_ms(DD);
	set_pin_state(DIGITAL_HIGH, CLK_PID);
	_delay_ms(DD);
	set_pin_state(DIGITAL_LOW, CLK_PID);
	_delay_ms(DD);
	set_pin_state(DIGITAL_LOW, DAT_PID);
	_delay_ms(DD);
}
void read(mdl_u8_t *__bstate) {
	set_pin_mode(DIGITAL_HIGH, DAT_PID);
	set_pin_mode(DIGITAL_INPUT, DAT_PID);
	_delay_ms(DD);
	*__bstate = get_pin_state(DAT_PID);
	_delay_ms(DD);
	set_pin_state(DIGITAL_HIGH, CLK_PID);
	_delay_ms(DD);
	set_pin_state(DIGITAL_LOW, CLK_PID);
	_delay_ms(DD);
//	scomm_print(&scomm, *__bstate? "READ: HIGH\n" : "READ: LOW\n");
	_delay_ms(DD);
	set_pin_mode(DIGITAL_OUTPUT, DAT_PID);
}

# define check_ack(__ack, ERR) \
if (__ack) { \
	set_pin_state(DIGITAL_HIGH, ERR_PID); \
	_delay_ms(1000); \
	set_pin_state(DIGITAL_LOW, ERR_PID); \
	scomm_print(&scomm, ERR); \
	return 1; \
}

void start_bit() {
	set_pin_state(DIGITAL_OUTPUT, DAT_PID);
	set_pin_state(DIGITAL_HIGH, DAT_PID);
	set_pin_state(DIGITAL_HIGH, CLK_PID);
	_delay_ms(500);


	set_pin_state(DIGITAL_LOW, DAT_PID);
	_delay_ms(DD);
	set_pin_state(DIGITAL_LOW, CLK_PID);
	_delay_ms(DD);
}

void end_bit() {
	set_pin_mode(DIGITAL_OUTPUT, DAT_PID);
	set_pin_state(DIGITAL_LOW, DAT_PID);
	set_pin_state(DIGITAL_HIGH, CLK_PID);
	_delay_ms(500);

	set_pin_state(DIGITAL_HIGH, DAT_PID);
	_delay_ms(DD);
}

mdl_u8_t send_addr(mdl_u16_t __addr) {
	mdl_u8_t ack_val = 0;
	for (mdl_u8_t o = 0; o != 8; o ++) {
		write((__addr >> (8 - o)) & 1);
	}

	read(&ack_val);
	for (mdl_u8_t o = 0; o != 8; o ++) {
		write((__addr >> (16 - (o + 8))) & 1);
	}

	read(&ack_val);
	return ack_val;
}

mdl_u8_t write_op(mdl_u8_t __d) {
	mdl_u8_t ack_val = 0;
	start_bit();

	// control code
	write(1);
	write(0);
	write(1);
	write(0);

	// chip select bits
	write(0);
	write(0);
	write(0);

	// r/w bit
	write(0);

	read(&ack_val);
//	check_ack(ack_val, "WRITE_OP, STAGE 0\n");

	ack_val = send_addr(0x50);
//	check_ack(ack_val, "WRITE_OP, STAGE 1\n");

	for (mdl_uint_t o = 0; o != 8; o ++) {write((__d >> o) & 1);}
	read(&ack_val);
//	check_ack(ack_val, "WRITE_OP, STAGE 2\n");

	end_bit();
	return 0;
}


mdl_u8_t read_op(mdl_u8_t *__d) {
	mdl_u8_t ack_val = 0;
	start_bit();

	// control code
	write(1);
	write(0);
	write(1);
	write(0);

	// chip select bits
	write(0);
	write(0);
	write(0);

	// r/w bit
	write(0);

	read(&ack_val);
//	check_ack(ack_val, "READ_OP, STAGE 0\n");

	ack_val = send_addr(0x50);
//	check_ack(ack_val, "READ_OP, STAGE 1\n");

	start_bit();

	// control code
	write(1);
	write(0);
	write(1);
	write(0);

	// chip select bits
	write(0);
	write(0);
	write(0);

	// r/w bit
	write(1);

	read(&ack_val);
//	check_ack(ack_val, "READ_OP, STAGE 2\n");

	set_pin_state(DIGITAL_HIGH, ERR_PID);
//	scomm_print(&scomm, "----------------------\n");

	for (mdl_u8_t o = 0; o != 8; o ++) {
		mdl_u8_t val = 0;
		read(&val);
		*__d |= val << o;
	}
	set_pin_state(DIGITAL_LOW, ERR_PID);

	read(&ack_val);
//	check_ack(ack_val, "READ_OP, STAGE 3\n");

	end_bit();
	return 0;
}

void micro_init() {
	scomm_init(&scomm, 0, 1);
	set_pin_mode(DIGITAL_OUTPUT, ERR_PID);
	set_pin_mode(DIGITAL_OUTPUT, A0_PID); // A0
	set_pin_mode(DIGITAL_OUTPUT, A1_PID); // A1
	set_pin_mode(DIGITAL_OUTPUT, A2_PID); // A2
	set_pin_mode(DIGITAL_OUTPUT, DAT_PID); // DATA
	set_pin_mode(DIGITAL_OUTPUT, CLK_PID); // CLOCK
	set_pin_state(DIGITAL_HIGH, DAT_PID);
	set_pin_state(DIGITAL_HIGH, CLK_PID);
}

# include "librarys/bin_str.h"
void micro_tick() {
	mdl_u8_t o = 0, control_byte = 0;
	scomm_recv_byte(&scomm, &control_byte);

	if (!o) {
		switch(control_byte) {
			case 'w': {
				mdl_u8_t data_byte = 0;
				scomm_recv_byte(&scomm, &data_byte);
				o = write_op(data_byte);
				scomm_print(&scomm, _w8_bin_str(data_byte));
				scomm_print(&scomm, "\n");
				break;
			}
			case 'r': {
				mdl_u8_t data_byte = 0;
				o = read_op(&data_byte);
				scomm_print(&scomm, _w8_bin_str(data_byte));
				scomm_print(&scomm, "\n");
				break;
			}
		}
	} else {
		scomm_print(&scomm, "error\n");
		o = 0;
	}
}
