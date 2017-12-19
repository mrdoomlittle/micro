# include "micro.h"
# ifndef __MICRO_LIGHT
# include "shift_reg.h"
struct shift_reg_t** shift_in_indx = NULL;
struct shift_reg_t** shift_out_indx = NULL;
mdl_u8_t shift_iindx_size = 0;
mdl_u8_t shift_oindx_size = 0;

void shift_indx_add(struct shift_reg_t*** __shift_indx, mdl_u8_t *__shift_indx_size, struct shift_reg_t** __shift_reg) {
	if (*__shift_indx_size == 0)
		*__shift_indx = malloc(sizeof(struct shift_reg_t*));
	else
		*__shift_indx = realloc(*__shift_indx, (*__shift_indx_size + 1) * sizeof(struct shift_reg_t*));

	struct shift_reg_t* shift_reg = malloc(sizeof(struct shift_reg_t));
	*__shift_reg = shift_reg;
	shift_reg-> inited = 0;
	(*__shift_indx)[*__shift_indx_size] = shift_reg;
	(*__shift_indx_size)++;
}

void shift_indx_rm(struct shift_reg_t*** __shift_indx, mdl_u8_t *__shift_indx_size, struct shift_reg_t *__shift_reg) {
	mdl_u8_t fnd_shift_reg = 0;
	for (mdl_u8_t point = 0; point != *__shift_indx_size - 1; point ++) {
		if (fnd_shift_reg) (*__shift_indx)[point - 1] = (*__shift_indx)[point];
		if (__shift_reg == (*__shift_indx)[point] && !fnd_shift_reg) {
			free((*__shift_indx)[point]);
			fnd_shift_reg = 1;
		}
	}

	if (*__shift_indx_size == 1)
		free(*__shift_indx);
	else
		*__shift_indx = realloc(*__shift_indx, (*__shift_indx_size - 1) * sizeof(struct shift_reg_t*));
	*__shift_indx_size--;
}
# endif

void
set_pin_mode(mdl_u8_t __pmode, mdl_u8_t __pid)
{
# ifdef __atmega328p
    if (__pid >= 0 && __pid <= 7)
    {
        if (__pmode == DIGITAL_OUTPUT)
            DDRD |= 1 << __pid;
        else if (__pmode == DIGITAL_INPUT)
            DDRD &= ~(1 << __pid);
    }
    else if (__pid > 7 && __pid <= 13)
    {
        if (__pmode == DIGITAL_OUTPUT)
            DDRB |= 1 << (__pid - 8);
        else if (__pmode == DIGITAL_INPUT)
            DDRB &= ~(1 << (__pid - 8));
    }
# endif
# ifdef __attiny85
	if (__pid >= 0 && __pid <= 4)
	{
		if (__pmode == DIGITAL_OUTPUT)
			DDRB |= 1 << __pid;
		else if (__pmode == DIGITAL_INPUT)
			DDRB &= ~(1 << __pid);
	}
# endif
}

# ifndef __MICRO_LIGHT
mdl_u8_t get_shift_indx_pnt(mdl_u8_t __pid) {
# ifdef __atmega328p
    mdl_u8_t shift_indx_pnt = floor((__pid - ATMEGA_329P_MX_PID)/8);
# endif

# ifdef __attiny85
	mdl_u8_t shift_indx_pnt = floor((__pid - ATTINY_85_MX_PID)/8);
# endif
	return shift_indx_pnt;
}

mdl_u8_t get_shift_pid(mdl_u8_t __pid) {
	mdl_u8_t shift_indx_pnt = get_shift_indx_pnt(__pid);
# ifdef __atmega328p
    return (__pid - ATMEGA_329P_MX_PID) - (shift_indx_pnt * 8) - 1;
# endif

# ifdef __attiny85
    return (__pid - ATTINY_85_MX_PID) - (shift_indx_pnt * 8) - 1;
# endif
}

void
sps_overflow(mdl_u8_t __pstate, mdl_u8_t __pid) {
	mdl_u8_t shift_indx_pnt = get_shift_indx_pnt(__pid);
	mdl_u8_t shift_pid = get_shift_pid(__pid);

	struct shift_reg_t* shift_reg = shift_out_indx[shift_indx_pnt];
	if (!shift_reg-> inited) return;

	shift_reg_sset(shift_reg, shift_pid, __pstate);
}
# endif

void
set_pin_state(mdl_u8_t __pstate, mdl_u8_t __pid)
{
# ifdef __atmega328p
    if (__pid >= 0 && __pid <= 7)
    {
        if (__pstate == DIGITAL_HIGH)
            PORTD |= 1 << __pid;
        else if (__pstate == DIGITAL_LOW)
            PORTD &= ~(1 << __pid);
    }
    else if (__pid > 7 && __pid <= 13)
    {
        if (__pstate == DIGITAL_HIGH)
            PORTB |= 1 << (__pid - 8);
        else if (__pstate == DIGITAL_LOW)
            PORTB &= ~(1 << (__pid - 8));
# ifndef __MICRO_LIGHT
    } else
		sps_overflow(__pstate, __pid);
# else
	}
# endif
# endif
# ifdef __attiny85
	if (__pid >= 0 && __pid <= 4)
	{
		if (__pstate == DIGITAL_HIGH)
			PORTB |= 1 << __pid;
		else if (__pstate == DIGITAL_LOW)
			PORTB &= ~(1 << __pid);
# ifndef __MICRO_LIGHT
	} else
		sps_overflow(__pstate, __pid);
# else
	}
# endif
# endif
}

# ifndef __MICRO_LIGHT
mdl_u8_t gps_overflow(mdl_u8_t __pid) {
	mdl_u8_t shift_indx_pnt = get_shift_indx_pnt(__pid);
	mdl_u8_t shift_pid = get_shift_pid(__pid);

	struct shift_reg_t* shift_reg = shift_in_indx[shift_indx_pnt];
	if (!shift_reg-> inited) return 0;

	mdl_u8_t pstate;
	shift_reg_sget(shift_reg, shift_pid, &pstate);
	return pstate;
}
# endif

mdl_u8_t
get_pin_state(mdl_u8_t __pid)
{
# ifdef __atmega328p
    if (__pid >= 0 && __pid <= 7)
    {
        mdl_u8_t val = PIND;
        return (((val >> __pid) & 1) == 1? 1 : 0);
    }
    else if (__pid > 7 && __pid <= 13)
    {
        mdl_u8_t val = PINB;
        return (((val >> (__pid - 8)) & 1) == 1? 1 : 0);
# ifndef __MICRO_LIGHT
    } else
		gps_overflow(__pid);
# else
	}
# endif
# endif
# ifdef __attiny85
	if (__pid >= 0 && __pid <= 4) {
		mdl_u8_t val = PINB;
		return (((val >> __pid) & 1) == 1? 1 : 0);
# ifndef __MICRO_LIGHT
	} else
		gps_overflow(__pid);
# else
	}
# endif
# endif
	return 0;
}
