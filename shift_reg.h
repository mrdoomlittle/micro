# ifndef __shift__reg__h
# define __shift__reg__h
# include <mdlint.h>
# include "micro.h"

# define SHIFT_REG_IN 0x0
# define SHIFT_REG_OUT 0x1
struct shift_reg_t {
	mdl_u8_t data_pid, latch_pid, clock_pid, data_flow, inited;
	mdl_u8_t ps_list[8];
};

void shift_reg_init(struct shift_reg_t*);
void shift_reg_set(struct shift_reg_t*, mdl_u8_t[8]);
void shift_reg_get(struct shift_reg_t*, mdl_u8_t[8]);

void shift_reg_sset(struct shift_reg_t*, mdl_u8_t, mdl_u8_t);
void shift_reg_sget(struct shift_reg_t*, mdl_u8_t, mdl_u8_t*);

void __inline__ static shift_reg_reset(struct shift_reg_t *__shift_reg, mdl_u8_t __ps[8]) {bzero(__ps, 8);}
void __inline__ static shift_reg_ireset(struct shift_reg_t *__shift_reg) {shift_reg_reset(__shift_reg, __shift_reg-> ps_list);}
# endif /*__shift__reg__h*/
