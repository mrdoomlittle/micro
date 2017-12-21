# include "shift_reg.h"
void shift_reg_init(struct shift_reg_t *__shift_reg) {
	if (__shift_reg->data_flow == SHIFT_REG_OUT)
		io_set_direct(io_direct_out, __shift_reg->data_pid);
	else if (__shift_reg->data_flow == SHIFT_REG_IN)
		io_set_direct(io_direct_in, __shift_reg->data_pid);

	io_set_direct(io_direct_out, __shift_reg->latch_pid);
	io_set_direct(io_direct_out, __shift_reg->clock_pid);

	if (__shift_reg->data_flow == SHIFT_REG_OUT)
		io_set_val(io_val_low, __shift_reg->data_pid);
	io_set_val(io_val_low, __shift_reg->latch_pid);
	io_set_val(io_val_low, __shift_reg->clock_pid);
	shift_reg_ireset(__shift_reg);
	__shift_reg->inited = 1;
}

void shift_reg_set(struct shift_reg_t *__shift_reg, mdl_u8_t __ps[8]) {
	if (__shift_reg->data_flow == SHIFT_REG_IN) return;
	mdl_u8_t pin = 0;
	for (;pin != 8;pin++) {
		io_set_val(__ps[7-pin], __shift_reg->data_pid);

		io_set_val(io_val_high, __shift_reg->clock_pid);
		_delay_ms(2);
		io_set_val(io_val_low, __shift_reg->clock_pid);
		_delay_ms(2);
	}

	io_set_val(io_val_high, __shift_reg->latch_pid);
	_delay_ms(2);
	io_set_val(io_val_low, __shift_reg->latch_pid);
	_delay_ms(2);
}

void shift_reg_get(struct shift_reg_t *__shift_reg, mdl_u8_t __ps[8]) {
	if (__shift_reg->data_flow == SHIFT_REG_OUT) return;
	io_set_val(io_val_high, __shift_reg->latch_pid);
	_delay_ms(2);
	io_set_val(io_val_low, __shift_reg->latch_pid);
	_delay_ms(2);

	mdl_u8_t pin = 0;
	for (;pin != 8;pin++) {
		__ps[7-pin] = io_get_val(__shift_reg->data_pid);
		io_set_val(io_val_high, __shift_reg->clock_pid);
		_delay_ms(2);
		io_set_val(io_val_low, __shift_reg->clock_pid);
		_delay_ms(2);
	}
}

void shift_reg_sset(struct shift_reg_t *__shift_reg, mdl_u8_t __pid, mdl_u8_t __ps) {
	if (__shift_reg->data_flow == SHIFT_REG_IN) return;
	__shift_reg->ps_list[__pid] = __ps;
	shift_reg_set(__shift_reg, __shift_reg->ps_list);
}


void shift_reg_sget(struct shift_reg_t *__shift_reg, mdl_u8_t __pid, mdl_u8_t *__ps) {
	if (__shift_reg->data_flow == SHIFT_REG_OUT) return;
	shift_reg_get(__shift_reg, __shift_reg->ps_list);
	*__ps = __shift_reg->ps_list[__pid];
}
