# ifndef __micro__h
# define __micro__h
# define io_direct_in 0x0
# define io_direct_out 0x1
# define io_val_high 0x1
# define io_val_low 0x0

# define __MICRO_LIGHT
# define ATTINY_85_MX_PID 4
# define ATMEGA_329P_MX_PID 13
# include "binary.h"
# include <stdint.h>
# include <stdlib.h>
# include <mdlint.h>
# include <avr/io.h>
# include <math.h>
# include <util/delay.h>
# include "uart.h"
# include "io.h"
# ifdef __mdl_tmp
#	include "tmp_io.h"
# endif

void micro_init();
void micro_tick();
void __inline__ static bzero(mdl_u8_t *__p, mdl_uint_t __bytes) {
	mdl_u8_t *itr = __p;
	while(itr != __p+__bytes)
		*(itr++) = 0x0;
}

# endif /*__micro__h*/
