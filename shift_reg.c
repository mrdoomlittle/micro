# include "shift_reg.h"
void shift_reg_init(struct shift_reg_t *__shift_reg) {
	if (__shift_reg-> data_flow == SHIFT_REG_OUT)
		set_pin_mode(DIGITAL_OUTPUT, __shift_reg-> data_pid);
	else if (__shift_reg-> data_flow == SHIFT_REG_IN)
		set_pin_mode(DIGITAL_INPUT, __shift_reg-> data_pid);

	set_pin_mode(DIGITAL_OUTPUT, __shift_reg-> latch_pid);
	set_pin_mode(DIGITAL_OUTPUT, __shift_reg-> clock_pid);

	if (__shift_reg-> data_flow == SHIFT_REG_OUT)
		set_pin_state(DIGITAL_LOW, __shift_reg-> data_pid);
	set_pin_state(DIGITAL_LOW, __shift_reg-> latch_pid);
	set_pin_state(DIGITAL_LOW, __shift_reg-> clock_pid);
	shift_reg_ireset(__shift_reg);
	__shift_reg-> inited = 1;
}

void shift_reg_set(struct shift_reg_t *__shift_reg, mdl_u8_t __ps[8]) {
	if (__shift_reg-> data_flow == SHIFT_REG_IN) return;
	for (mdl_u8_t pid = 0; pid != 8; pid ++) {
		set_pin_state(__ps[7 - pid], __shift_reg-> data_pid);

		set_pin_state(DIGITAL_HIGH, __shift_reg-> clock_pid);
		_delay_ms(2);
		set_pin_state(DIGITAL_LOW, __shift_reg-> clock_pid);
		_delay_ms(2);
	}

	set_pin_state(DIGITAL_HIGH, __shift_reg-> latch_pid);
	_delay_ms(2);
	set_pin_state(DIGITAL_LOW, __shift_reg-> latch_pid);
	_delay_ms(2);
}

void shift_reg_get(struct shift_reg_t *__shift_reg, mdl_u8_t __ps[8]) {
	if (__shift_reg-> data_flow == SHIFT_REG_OUT) return;
	set_pin_state(DIGITAL_HIGH, __shift_reg-> latch_pid);
	_delay_ms(2);
	set_pin_state(DIGITAL_LOW, __shift_reg-> latch_pid);
	_delay_ms(2);

	for (mdl_u8_t pid = 0; pid != 8; pid ++) {
		__ps[7 - pid] = get_pin_state(__shift_reg-> data_pid);
		set_pin_state(DIGITAL_HIGH, __shift_reg-> clock_pid);
		_delay_ms(2);
		set_pin_state(DIGITAL_LOW, __shift_reg-> clock_pid);
		_delay_ms(2);
	}
}

void shift_reg_sset(struct shift_reg_t *__shift_reg, mdl_u8_t __pid, mdl_u8_t __ps) {
	if (__shift_reg-> data_flow == SHIFT_REG_IN) return;
	__shift_reg-> ps_list[__pid] = __ps;
	shift_reg_set(__shift_reg, __shift_reg-> ps_list);
}


void shift_reg_sget(struct shift_reg_t *__shift_reg, mdl_u8_t __pid, mdl_u8_t *__ps) {
	if (__shift_reg-> data_flow == SHIFT_REG_OUT) return;
	shift_reg_get(__shift_reg, __shift_reg-> ps_list);
	*__ps = __shift_reg-> ps_list[__pid];
}
