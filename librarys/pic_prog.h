# ifndef __pic__prog__h
# define __pic__prog__h
# include "../micro.h"
# include <eint_t.h>
# include "../uart.h"
# include "bin_str.h"
struct pic_prog_t {
    mdl_u8_t clk_pid, data_pid, pgm_pid, mclr_pid;
};

void pic_prog_init(struct pic_prog_t*, mdl_u8_t, mdl_u8_t, mdl_u8_t, mdl_u8_t);
void pic_prog_tick(struct pic_prog_t*);
void pic_prog_inc_addr_cm(struct pic_prog_t*);
void pic_prog_send(struct pic_prog_t*, mdl_u8_t, mdl_uint_t);
void pic_prog_recv(struct pic_prog_t*, mdl_u8_t*, mdl_uint_t);
void pic_prog_write_bit(struct pic_prog_t*, mdl_u8_t);
void pic_prog_read_bit(struct pic_prog_t*, mdl_u8_t *);
void pic_prog_read_pm(struct pic_prog_t*, mdl_u16_t *);
void pic_prog_write_pm(struct pic_prog_t*, mdl_u16_t);
void pic_prog_begin_epc_cm(struct pic_prog_t*);
void pic_prog_begin_poc_cm(struct pic_prog_t*);
void pic_prog_ld_fpm_cm(struct pic_prog_t*);
void pic_prog_rd_fpm_cm(struct pic_prog_t*);
void pic_prog_ld_fdm_cm(struct pic_prog_t*);
void pic_prog_rd_fdm_cm(struct pic_prog_t*);
void pic_prog_load_conf_cm(struct pic_prog_t*);
void pic_prog_bulk_er_s1_cm(struct pic_prog_t*);
void pic_prog_bulk_er_s2_cm(struct pic_prog_t*);
# endif /*__pic__prog__h*/
