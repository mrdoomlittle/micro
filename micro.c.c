# include "micro.h"
# include <tmp_io.h>

# define TMP_RX_IC_PID 2
# define TMP_RX_OC_PID 3

# define TMP_TX_IC_PID 4
# define TMP_TX_OC_PID 5

# define TMP_RX_PID 6
# define TMP_TX_PID 7
//# define __SERVER
//# define __CLIENT
//# define AT
//# define TA
# include "shift_reg.h"
# include "serial_comm.h"
# include "librarys/pic_prog.h"
# if !defined(__CLIENT) && !defined(__SERVER) && defined(AT)
#	include <avr/interrupt.h>
mdl_u8_t static val = 0;
# endif
int main(void)
{
    _delay_ms(5000);
# ifdef __SERVER
	struct tmp_io_t _tmp_io = {
		.rx_pid = TMP_RX_PID,
		.tx_pid = TMP_TX_PID,
		.rx_ci_pid = TMP_RX_IC_PID,
		.tx_ci_pid = TMP_TX_IC_PID,
		.rx_co_pid = TMP_RX_OC_PID,
		.tx_co_pid = TMP_TX_OC_PID
	};

	tmp_init(&_tmp_io, &set_pin_mode, &set_pin_state, &get_pin_state);
	mdl_uint_t timeo = 2000000;
	tmp_set_opt(&_tmp_io, TMP_OPT_SND_TIMEO, &timeo);
	tmp_set_opt(&_tmp_io, TMP_OPT_RCV_TIMEO, &timeo);
# endif
# ifdef __CLIENT
	struct shift_reg_t *shift_reg;
	shift_indx_add(&shift_out_indx, &shift_oindx_size, &shift_reg);
	shift_reg-> data_pid = 0;
	shift_reg-> latch_pid = 3;
	shift_reg-> clock_pid = 4;
	shift_reg-> data_flow = SHIFT_REG_OUT;
	shift_reg_init(shift_reg);

	shift_reg = NULL;
	shift_indx_add(&shift_in_indx, &shift_iindx_size, &shift_reg);
	shift_reg-> data_pid = 1;
	shift_reg-> latch_pid = 5;
	shift_reg-> clock_pid = 6;
	shift_reg-> data_flow = SHIFT_REG_IN;
	shift_reg_init(shift_reg);

	struct tmp_io_t _tmp_io = {
		.rx_pid = 7,
		.tx_pid = 9,
		.rx_ci_pid = 5,
		.tx_ci_pid = 6,
		.rx_co_pid = 7,
		.tx_co_pid = 8
	};

	tmp_init(&_tmp_io, &set_pin_mode, &set_pin_state, &get_pin_state);
	mdl_uint_t timeo = 20;
	tmp_set_opt(&_tmp_io, TMP_OPT_SND_TIMEO, &timeo);
	tmp_set_opt(&_tmp_io, TMP_OPT_RCV_TIMEO, &timeo);
	_delay_ms(1000);
# endif
# if !defined(__CLIENT) && !defined(__SERVER) && defined(AT)

	ADCSRA |= 1 << ADEN;
	ADCSRA |= 1 << ADPS2;
	ADCSRA |= 1 << ADPS1;
	ADCSRA |= 1 << ADPS0;

	ADMUX |= 1 << ADLAR;
	ADCSRA |= 1 << ADIE;
//	ADMUX |= 1 << REFS0;

	sei();

	ADCSRA |= 1 << ADSC;

	struct scomm_t scomm;
	scomm_init(&scomm, 0, 1);

	mdl_u8_t last_state = DIGITAL_LOW;
	mdl_uint_t change_c = 0;
	mdl_uint_t check_am = 10;
	mdl_uint_t high_c = 0, low_c = 0;
	mdl_uint_t high_val = 255/2, timer = 0;
# endif
# if !defined(__CLIENT) && !defined(__SERVER) && !defined(AT) && !defined(TA)
	mdl_u8_t ce_pid = 5, oe_pid = 6, we_pid = 7, dq_pid = 8, a_pid = 9;
	set_pin_mode(DIGITAL_OUTPUT, ce_pid);
	set_pin_mode(DIGITAL_OUTPUT, oe_pid);
	set_pin_mode(DIGITAL_OUTPUT, we_pid);
	set_pin_mode(DIGITAL_OUTPUT, a_pid);

	struct scomm_t scomm;
	scomm_init(&scomm, 0, 1);
# endif
# ifdef TA
	struct scomm_t scomm;
	scomm_init(&scomm, 0, 1);
	struct pic_prog_t pic_prog;
	pic_prog_init(&pic_prog, 2, 3, 4, 5);
# endif
    for (;;) {
# ifdef TA
		pic_prog_tick(&pic_prog, &scomm);
# endif

# if !defined(__CLIENT) && !defined(__SERVER) && defined(AT)
		mdl_u8_t state;
		if (val > high_val) {high_c++;state=DIGITAL_HIGH;} else {low_c++;state=DIGITAL_LOW;}

		if (last_state != state) {
			change_c++;
			last_state = state;
		}

		if (timer < 1000000/2) {timer++; continue;}
		scomm_send(&scomm, (mdl_u8_t*)&high_c, sizeof(mdl_uint_t));
		scomm_send(&scomm, (mdl_u8_t*)&low_c, sizeof(mdl_uint_t));
		scomm_send(&scomm, (mdl_u8_t*)&change_c, sizeof(mdl_uint_t));
		high_c = low_c = change_c = timer = 0;
# endif
# if !defined(__CLIENT) && !defined(__SERVER) && !defined(AT) && !defined(TA)
		mdl_u8_t val = 0x0;
		scomm_recv_byte(&scomm, &val);
		set_pin_mode(DIGITAL_OUTPUT, dq_pid);

		val = 0x0;
		set_pin_mode(DIGITAL_INPUT, dq_pid);

		scomm_send_byte(&scomm, val);
# endif
# ifdef __CLIENT
		mdl_u8_t any_err = TMP_SUCCESS;
		mdl_u8_t recved_byte = 0x0;
		any_err = tmp_recv_byte(&_tmp_io, &recved_byte);
		set_pin_state(recved_byte, 10);
		if (any_err != TMP_SUCCESS)
			set_pin_state(DIGITAL_HIGH, 11);
		else
			set_pin_state(DIGITAL_LOW, 11);
		_delay_ms(500);
		set_pin_state(DIGITAL_LOW, 11);
# endif

# ifdef __SERVER
		tmp_send_byte(&_tmp_io, 0x1);
		_delay_ms(500);
		tmp_send_byte(&_tmp_io, 0x0);
		_delay_ms(500);
# endif
//		_delay_ms(1000);
    }
    return 0;
}
# if !defined(__CLIENT) && !defined(__SERVER) && defined(AT)
ISR(ADC_vect) {
	val = ADCH;
	ADCSRA |= 1 << ADSC;
}
# endif
