# ifndef __8b__var__hpp
# define __8b__var__hpp
# define READER 0
# define WRITER 1
# ifdef ARDUINO
void set_pin_mode(uint8_t __pmode, uint8_t __pid) {
	pinMode(__pid, __pmode);
}

void set_pin_state(uint8_t __pstate, uint8_t __pid) {
	digitalWrite(__pid, __pstate);
}

uint8_t get_pin_state(uint8_t __pid) {
	return digitalRead(__pid);
}
# define DIGITAL_HIGH HIGH
# define DIGITAL_LOW LOW
# define DIGITAL_OUTPUT OUTPUT
# define DIGITAL_INPUT INPUT
# endif

uint8_t rw_type = 0, * pin_ids;
void b8init(uint8_t __type, uint8_t * __pids) {
	rw_type = __type;
	pin_ids = __pids;
	if (__type == READER) {
		for (size_t i = 0; i != 8; i ++)
			set_pin_mode(DIGITAL_INPUT, __pids[i]);
	}

	if (__type == WRITER) {
		for (size_t i = 0; i != 8; i ++)
			set_pin_mode(DIGITAL_OUTPUT, __pids[i]);
	}
}

uint8_t b8set(uint8_t __val) {

	for (size_t i = 0; i != 8; i ++) {
		uint8_t pin_state = 0;

		pin_state = ((__val >> i) & 1) == 1? DIGITAL_HIGH : DIGITAL_LOW;

		set_pin_state(pin_state, pin_ids[i]);
	}

	return 0;
}

uint8_t b8get() {
	uint8_t return_val = 0;

	for (size_t i = 0; i != 8; i ++) {
		if (get_pin_state(pin_ids[i]) == DIGITAL_HIGH) return_val |= 1 << i;
	}

	return return_val;
}



# endif 
