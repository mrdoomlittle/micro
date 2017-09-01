# ifndef __micro__h
# define __micro__h
# define DIGITAL_HIGH 0x1
# define DIGITAL_LOW 0x0

# define DIGITAL_OUTPUT 0x1
# define DIGITAL_INPUT 0x0

# define DIG_GPIO_HIGH 0x1
# define DIG_GPIO_LOW 0x0

# define DIG_GPIO_OUT 0x1
# define DIG_GPIO_IN 0x0

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
# include "wiring_digital.h"

# ifdef __WITH_TMP
#	include "tmp_io.h"
# endif

void extern micro_init();
void extern micro_tick();

void __inline__ bzero(mdl_u8_t *__data, mdl_uint_t __bytes) {
	for (mdl_uint_t point = 0; point != __bytes; point ++) *(__data + point) = 0x0;
}

# endif /*__micro__h*/
