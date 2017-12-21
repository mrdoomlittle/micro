# include "micro.h"
struct tmp_io _tmp_io = {
	.rx_pid = TMP_RX_PID,
	.tx_pid = TMP_TX_PID,
	.rx_ci_pid = TMP_RX_IC_PID,
	.tx_ci_pid = TMP_TX_IC_PID,
	.rx_co_pid = TMP_RX_OC_PID,
	.tx_co_pid = TMP_TX_OC_PID
};

void micro_holdup(mdl_uint_t __holdup) {
	if (__holdup == 1) {_delay_ms(1);} else {_delay_ms(0.02);}}

void _tmp_init() {
	tmp_init(&_tmp_io, &io_set_direct, &io_set_val, &io_get_val, &micro_holdup);

	// invert the clock signals
	tmp_tog_snd_optflag(&_tmp_io, TMP_OPT_INV_TX_TRIG_VAL);
	tmp_tog_rcv_optflag(&_tmp_io, TMP_OPT_INV_RX_TRIG_VAL);

	// flip each bit recved thru the rx line
	tmp_tog_rcv_optflag(&_tmp_io, TMP_OPT_FLIP_BIT);
	_tmp_io.snd_holdup_ic = 11;
	_tmp_io.rcv_holdup_ic = 11;

	mdl_uint_t timeo = 12000; //ms
	tmp_set_opt(&_tmp_io, TMP_OPT_SND_CUTOFF, &timeo);
	tmp_set_opt(&_tmp_io, TMP_OPT_RCV_CUTOFF, &timeo);
}
