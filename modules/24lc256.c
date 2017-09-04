# include "24lc256.h"
# define ISR_DISABLE mdl_u8_t re_enable_gi = 0; if ((SREG >> 7) & 1) {re_enable_gi = 1; cli();}
# define ISR_ENABLE if (re_enable_gi) sei();

void _24lc256_start_bit(struct _24lc256_t *__24lc256) {
	ISR_DISABLE
	set_pin_state(DIGITAL_OUTPUT, __24lc256->sda_pid);
	set_pin_state(DIGITAL_HIGH, __24lc256->sda_pid);
	set_pin_state(DIGITAL_HIGH, __24lc256->scl_pid);
	_delay_us(_24LC256_LDELAY);

	set_pin_state(DIGITAL_LOW, __24lc256->sda_pid);
	_delay_us(_24LC256_DELAY);
	set_pin_state(DIGITAL_LOW, __24lc256->scl_pid);
	_delay_us(_24LC256_DELAY);
	ISR_ENABLE
}

void _24lc256_end_bit(struct _24lc256_t *__24lc256) {
	ISR_DISABLE
	set_pin_mode(DIGITAL_OUTPUT, __24lc256->sda_pid);
	set_pin_state(DIGITAL_LOW, __24lc256->sda_pid);
	set_pin_state(DIGITAL_HIGH, __24lc256->scl_pid);
	_delay_us(_24LC256_LDELAY);

	set_pin_state(DIGITAL_HIGH, __24lc256->sda_pid);
	_delay_us(_24LC256_DELAY);
	ISR_ENABLE
}

void _24lc256_write_bit(struct _24lc256_t *__24lc256, mdl_u8_t __bit) {
	ISR_DISABLE
	set_pin_mode(DIGITAL_OUTPUT, __24lc256->sda_pid);
	set_pin_state(__bit, __24lc256->sda_pid);

	set_pin_state(DIGITAL_HIGH, __24lc256->scl_pid);
	_delay_us(_24LC256_CLK_HI_DELAY);
	set_pin_state(DIGITAL_LOW, __24lc256->scl_pid);
	_delay_us(_24LC256_CLK_LO_DELAY);
	set_pin_state(DIGITAL_LOW, __24lc256->sda_pid);
	_delay_us(_24LC256_DELAY);
	ISR_ENABLE
}

void _24lc256_read_bit(struct _24lc256_t *__24lc256, mdl_u8_t *__bit) {
	ISR_DISABLE
	set_pin_mode(DIGITAL_HIGH, __24lc256->sda_pid);
	set_pin_mode(DIGITAL_INPUT, __24lc256->sda_pid);
	_delay_us(_24LC256_DELAY);
	*__bit = get_pin_state(__24lc256->sda_pid);

	set_pin_state(DIGITAL_HIGH, __24lc256->scl_pid);
	_delay_us(_24LC256_CLK_HI_DELAY);
	set_pin_state(DIGITAL_LOW, __24lc256->scl_pid);
	_delay_us(_24LC256_CLK_LO_DELAY);
	set_pin_mode(DIGITAL_OUTPUT, __24lc256->sda_pid);
	ISR_ENABLE
}

void _24lc256_send_addr(struct _24lc256_t *__24lc256, mdl_u16_t __addr) {
	mdl_u8_t ack_val = 0;
	for (mdl_u8_t o = 0; o != 8; o ++)
		_24lc256_write_bit(__24lc256, (__addr >> (15 - o)) & 0x1);

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);

	for (mdl_u8_t o = 0; o != 8; o ++)
		_24lc256_write_bit(__24lc256, (__addr >> (7 - o)) & 0x1);

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);
}


mdl_u8_t volatile ignore_end_bit = 0;
void _24lc256_put_byte(struct _24lc256_t *__24lc256, mdl_u8_t __byte, mdl_u16_t __addr) {
	mdl_u8_t ack_val = 0;
	// start bit
	_24lc256_start_bit(__24lc256);

	// control code
	_24lc256_write_bit(__24lc256, 1);
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 1);
	_24lc256_write_bit(__24lc256, 0);

	// chip select bits
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 0);

	// r/w bit
	_24lc256_write_bit(__24lc256, 0);

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);

	// send memory address
	_24lc256_send_addr(__24lc256, __addr);

	// write data to memory
	for (mdl_u8_t bit_fs = 0; bit_fs != 8; bit_fs++)
		_24lc256_write_bit(__24lc256, (__byte >> bit_fs) & 1);

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);

	// end bit
	if (!ignore_end_bit)
		_24lc256_end_bit(__24lc256);
	_delay_ms(6);
}

void _24lc256_page_write(struct _24lc256_t *__24lc256, mdl_u8_t __page[64], mdl_u16_t __addr) {
	ignore_end_bit = 1;
	_24lc256_put_byte(__24lc256, *__page, __addr);
	ignore_end_bit = 0;

	for (mdl_u8_t *itr = __page+1; itr != __page + 64; itr++) {
		for (mdl_u8_t bit_fs = 0; bit_fs != 8; bit_fs++)
			_24lc256_write_bit(__24lc256, (*itr >> bit_fs) & 1);

		mdl_u8_t ack_val = 0;
		_24lc256_read_bit(__24lc256, &ack_val); //ack
		_delay_ms(6);
	}

	// end bit
	_24lc256_end_bit(__24lc256);
	_delay_ms(6);
}

void _24lc256_get_byte(struct _24lc256_t *__24lc256, mdl_u8_t *__byte, mdl_u16_t __addr) {
	mdl_u8_t ack_val = 0;
	// start bit
	_24lc256_start_bit(__24lc256);

	// control code
	_24lc256_write_bit(__24lc256, 1);
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 1);
	_24lc256_write_bit(__24lc256, 0);

	// chip select bits
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 0);

	// r/w bit
	_24lc256_write_bit(__24lc256, 0);

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);

	// send memory address
	_24lc256_send_addr(__24lc256, __addr);

	// start bit
	_24lc256_start_bit(__24lc256);

	// control code
	_24lc256_write_bit(__24lc256, 1);
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 1);
	_24lc256_write_bit(__24lc256, 0);

	// chip select bits
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 0);
	_24lc256_write_bit(__24lc256, 0);

	// r/w bit
	_24lc256_write_bit(__24lc256, 1);

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);

	for (mdl_u8_t bit_fs = 0; bit_fs != 8; bit_fs ++) {
		mdl_u8_t recved_bit = 0;
		_24lc256_read_bit(__24lc256, &recved_bit);
		*__byte |= recved_bit << bit_fs;
	}

	// get ack
	_24lc256_read_bit(__24lc256, &ack_val);

	// end bit
	if (!ignore_end_bit)
		_24lc256_end_bit(__24lc256);
}

void _24lc256_seq_read(struct _24lc256_t *__24lc256, mdl_u8_t *__mem_ptr, mdl_uint_t __bc, mdl_u16_t __addr) {
	ignore_end_bit = 1;
	_24lc256_get_byte(__24lc256, __mem_ptr, __addr);
	ignore_end_bit = 0;

	for (mdl_u8_t *itr = __mem_ptr+1; itr != __mem_ptr + __bc; itr++) {
		for (mdl_u8_t bit_fs = 0; bit_fs != 8; bit_fs ++) {
			mdl_u8_t recved_bit = 0;
			_24lc256_read_bit(__24lc256, &recved_bit);
			*itr |= recved_bit << bit_fs;
		}

		mdl_u8_t ack_val = 0;
		_24lc256_read_bit(__24lc256, &ack_val); // ack
	}

	_24lc256_end_bit(__24lc256);
}

void _24lc256_init(struct _24lc256_t *__24lc256, mdl_u8_t __sda_pid, mdl_u8_t __scl_pid) {
	__24lc256->sda_pid = __sda_pid;
    __24lc256->scl_pid = __scl_pid;
	set_pin_mode(DIGITAL_OUTPUT, __24lc256->sda_pid);
	set_pin_mode(DIGITAL_OUTPUT, __24lc256->scl_pid);
	set_pin_state(DIGITAL_HIGH, __24lc256->sda_pid);
	set_pin_state(DIGITAL_HIGH, __24lc256->scl_pid);
}
