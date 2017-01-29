# define MCLOCK_PID 5
# define MLATCH_PID 6
# define PDATA_PID 8
# define NDATA_PID 7

# define DELAY 10
namespace display {

void init() {
    set_pin_mode(DIGITAL_OUTPUT, MCLOCK_PID);
    set_pin_mode(DIGITAL_OUTPUT, MLATCH_PID);
    set_pin_mode(DIGITAL_OUTPUT, PDATA_PID);
    set_pin_mode(DIGITAL_OUTPUT, NDATA_PID);
}
 
// 2, 0, 7, 1, 3, 6, 4, 5

uint8_t cols[8] = { 2, 0, 7, 1, 3, 6, 4, 5 };
uint8_t count = 0;
uint8_t output[8][8] = {
    { 0x1, 0x1, 0x1, 0x0, 0x0, 0x1, 0x1, 0x1 },
    { 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1 },
    { 0x1, 0x0, 0x1, 0x0, 0x0, 0x1, 0x0, 0x1 },
    { 0x0, 0x0, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0 },
    { 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 },
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
};

uint8_t rows[8] = { 6, 0, 1, 4, 2, 5, 7, 3 };

uint8_t limp(uint8_t __val) {
	if (__val < 0 || __val > (8 * 8)) return 1;

	uint8_t col = floor(__val / 8);
	uint8_t row = (__val - (col * 8));

	output[col][row] = 0x1;

	return 0;
}

void clear() {
	for (size_t i = 0; i != 8; i++)
		for (size_t o = 0; o != 8; o++)
			output[i][o] = 0x0;
}

void test() {
    set_pin_state(DIGITAL_LOW, PDATA_PID);
    set_pin_state(DIGITAL_LOW, NDATA_PID);

    for (size_t i = 0; i != 8; i++) {
        if (i == cols[count]) set_pin_state(DIGITAL_HIGH, PDATA_PID);
        for (size_t o = 0; o != 8; o++) {
            if (i == rows[o]) set_pin_state(output[count][o], NDATA_PID);
        }

        set_pin_state(DIGITAL_HIGH, MCLOCK_PID);
        if (DELAY != 0) _delay_ms(DELAY);
        set_pin_state(DIGITAL_LOW, MCLOCK_PID);
        if (DELAY != 0) _delay_ms(DELAY);
        set_pin_state(DIGITAL_LOW, PDATA_PID);
        set_pin_state(DIGITAL_LOW, NDATA_PID);
    }

    set_pin_state(DIGITAL_HIGH, MLATCH_PID);
    if (DELAY != 0) _delay_ms(DELAY);

    set_pin_state(DIGITAL_LOW, MLATCH_PID);

    set_pin_state(DIGITAL_LOW, PDATA_PID);
    set_pin_state(DIGITAL_LOW, NDATA_PID);
    if (DELAY != 0) _delay_ms(DELAY);
    if (count == 7) count = 0; else count ++;
}

}
