# ifndef __24lc256__h
# define __24lc256__h
# include <mdlint.h>
# include "../micro.h"
# include <avr/interrupt.h>
# define _24LC256_DELAY 1.4
# define _24LC256_CLK_HI_DELAY 0.7
# define _24LC256_CLK_LO_DELAY 1.4
# define _24LC256_LDELAY 1.8
struct _24lc256 {
	mdl_u8_t sda_pid, scl_pid;
};

void _24lc256_start_bit(struct _24lc256*);
void _24lc256_end_bit(struct _24lc256*);
void _24lc256_init(struct _24lc256*, mdl_u8_t, mdl_u8_t);

void _24lc256_write_bit(struct _24lc256*, mdl_u8_t);
void _24lc256_read_bit(struct _24lc256*, mdl_u8_t*);

void _24lc256_put_byte(struct _24lc256*, mdl_u8_t, mdl_u16_t);
void _24lc256_get_byte(struct _24lc256*, mdl_u8_t*, mdl_u16_t);

void _24lc256_page_write(struct _24lc256*, mdl_u8_t[64], mdl_u16_t);
void _24lc256_seq_read(struct _24lc256*, mdl_u8_t*, mdl_uint_t, mdl_u16_t);
# endif /*__24lc256__h*/
