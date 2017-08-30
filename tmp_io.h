# ifndef __micro__tmp__io__h
# define __micro__tmp__io__h

# define TMP_RX_IC_PID 5
// 5
# define TMP_RX_OC_PID 6

# define TMP_TX_IC_PID 7
# define TMP_TX_OC_PID 8

# define TMP_RX_PID 9
# define TMP_TX_PID 10

# include <tmp_io.h>
struct tmp_io extern _tmp_io;

void _tmp_init();
# endif
