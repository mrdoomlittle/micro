# ifndef __wiring__digital__hpp__
# define __wiring__digital__hpp__
extern void set_pin_mode(uint8_t __pmode, uint8_t __pid);
extern void set_pin_state(uint8_t __pstate, uint8_t __pid);
extern uint8_t get_pin_state(uint8_t __pid);
# endif /*__wiring__digital__hpp__*/
