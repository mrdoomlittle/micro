# include <tmp_io.h>

# define TMP_RX_IC_PID 2
# define TMP_RX_OC_PID 3

# define TMP_TX_IC_PID 4
# define TMP_TX_OC_PID 5

# define TMP_RX_PID 6
# define TMP_TX_PID 7

struct tmp_io_t tmp_io = {
	.rx_pid = TMP_RX_PID,
	.tx_pid = TMP_TX_PID,
	.rx_ci_pid = TMP_RX_IC_PID,
	.tx_ci_pid = TMP_TX_IC_PID,
	.rx_co_pid = TMP_RX_OC_PID,
	.tx_co_pid = TMP_TX_OC_PID
};

void micro_holdup(mdl_uint_t __holdup) {
	if (__holdup == 1) {_delay_ms(1);} else {_delay_ms(0.02);}}

void micro_init() {
	tmp_init(&tmp_io, &set_pin_mode, &set_pin_state, &get_pin_state);

	// invert the clock signals
	tmp_tog_snd_optflag(&tmp_io, TMP_INVERT_TX_TRIG_VAL_OPT);
	tmp_tog_rcv_optflag(&tmp_io, TMP_INVERT_RX_TRIG_VAL_OPT);

	// flip each bit recved thru the rx line
	tmp_tog_rcv_optflag(&tmp_io, TMP_FLIP_BIT_OPT);

	tmp_set_holdup_fptr(&tmp_io, &micro_holdup);
	tmp_io.snd_holdup_ic = 12;
	tmp_io.rcv_holdup_ic = 12;
	mdl_uint_t timeo = 12000; //ms
	tmp_set_opt(&tmp_io, TMP_OPT_SND_TIMEO, &timeo);
	tmp_set_opt(&tmp_io, TMP_OPT_RCV_TIMEO, &timeo);

	tmp_err_t any_err;
	tmp_set_iface_addr(&tmp_io, tmp_addr_from_str("192.168.0.1", &any_err));
}

void micro_tick() {
	mdl_u8_t outbound_byte = 'D';
//	uart_recv_byte(&outbound_byte);
	if (tmp_send(&tmp_io, tmp_io_buff(&outbound_byte, 1), 0) != TMP_SUCCESS) return;
}

