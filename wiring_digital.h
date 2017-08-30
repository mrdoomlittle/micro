# ifndef __wiring__digital__h
# define __wiring__digital__h
extern void set_pin_mode(mdl_u8_t __pmode, mdl_u8_t __pid);
extern void set_pin_state(mdl_u8_t __pstate, mdl_u8_t __pid);
extern mdl_u8_t get_pin_state(mdl_u8_t __pid);
# ifndef __MICRO_LIGHT
extern struct shift_reg_t** shift_in_indx;
extern struct shift_reg_t** shift_out_indx;
extern mdl_u8_t shift_iindx_size;
extern mdl_u8_t shift_oindx_size;
extern void shift_indx_add(struct shift_reg_t***, mdl_u8_t *, struct shift_reg_t**);
extern void shift_indx_rm(struct shift_reg_t***, mdl_u8_t *, struct shift_reg_t *);
# endif
# endif /*__wiring__digital__h*/
