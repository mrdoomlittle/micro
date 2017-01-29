# include "micro.hpp"
//# define DISPLAY
# define __SE

# include "serial_comm.hpp"
//# include "librarys/itmp.cpp"
# ifdef DISPLAY
# include <string.h>
# include "librarys/display.cpp"
# include "librarys/8b_var.hpp"
# endif
# ifndef DISPLAY
# include "/home/daniel-robson/Projects/tmp/io.h"
# endif
int main(void)
{
# ifdef DISPLAY
//	uint8_t pids[8] = { 2, 3, 4, 9, 10, 11, 12, 13 };
//	b8init(READER, pids);
# endif
/*

    ADCSRA |= 1 << ADPS2;
    ADMUX |= 1 << ADLAR;
    ADCSRA |= 1 << ADIE;
    ADCSRA |= 1 << ADEN;

    //sei();

    ADCSRA |= 1 << ADSC;
*/
    _delay_ms(5000);

    serial_comm scom(9600, 0, 1);
# ifdef DISPLAY
	display::init();
# endif
//    itmp::init();

//    set_pin_mode(DIGITAL_OUTPUT, 5);
# ifndef DISPLAY
	io __io;

	__io.set_pmode_fptr = &set_pin_mode;
    __io.set_pstate_fptr = &set_pin_state;
    __io.get_pstate_fptr = &get_pin_state;
    __io.already_inited = false;

	init(&__io, 8, 8);
	const uint8_t tip = 255;
	uint8_t x[2] = {tip, 0};
	uint8_t * y;
	_delay_ms(2000);
	bool ready = false;
	set_pin_mode(DIGITAL_INPUT, 9);
# endif
# ifdef DISPLAY
	size_t frame_point = 0, lframe_point = 0;
	size_t frame_size = (8 * 8);
	bool already_zcalled = false;
	display::clear();
# endif
    for (;;)
    {
# ifndef DISPLAY
# ifdef __SE
/*
		if (x[0] != 0)
            x[0] --;
        else
            x[0] = tip;
*/
		if (get_pin_state(9) == DIGITAL_LOW) ready = true;
		if (get_pin_state(9) == DIGITAL_HIGH && ready) {
			x[0]--;
			ready = false;
		}

		send(&__io, io_buff(x, 2));
		//scom.transmit_byte(x[0]);
# endif
# ifdef __CL
		recv(&__io, io_buff(y, 2));

		scom.transmit_byte(y[0]);
		free(y);
# endif
# endif
# ifdef DISPLAY
	for (size_t o = 0; o != 8; o ++)
        memset(display::output[o], 0x1, 8);
	display::test();
	scom.transmit_byte(display::output[0][0]);
/*
	uint8_t frame_tpoint = b8get();
	if (frame_tpoint != lframe_point) {
		if (frame_tpoint == 0) {
			//_delay_ms(100);
			display::clear();
			display::count = 0;
			for (size_t o = 0; o != 8; o++) display::test();
		}

		if (display::limp(frame_tpoint)) continue;
		scom.transmit_byte(frame_tpoint);

		if (frame_tpoint == frame_size || frame_tpoint > (frame_size / 2)) {
			display::count = 0;
			for (size_t o = 0; o != 8; o++) display::test();
		}

		lframe_point = frame_tpoint;
	}
*/
/*
	uint8_t vv = b8get();
	if (vv != last_frame_val || frame_count == 0) {
		if (frame_count == 0) {
			_delay_ms(100);
			display::clear();
		}

		display::limp(vv);

		if (frame_count == 64) {
        	frame_count = 0;
        	display::test();
    	} else frame_count ++;

		last_frame_val = vv;
	}
*/
# endif
//		#endif


//		set_pin_state(DIGITAL_HIGH, 8);
//		_delay_ms(1000);


//		set_pin_state(DIGITAL_LOW, 8);
//		_delay_ms(1000);
/*
        itmp::tick();

        if (itmp::i == '!') {
            set_pin_state(DIGITAL_HIGH, 5);
        } else {
            set_pin_state(DIGITAL_LOW, 5);
        }
*/

//        display::test();

/*
        // 1 000 000
        if (tick >= 1000000) {
          

            char adcResault[4];
       
            itoa(ADCH, adcResault, 10);

            char * __out = static_cast<char *>(malloc(5));
            __out[0] = adcResault[0];
            __out[1] = adcResault[1];
            __out[2] = adcResault[2];
            __out[3] = adcResault[3]; 
             
            scom.transmit(__out);
          

            ADCSRA |= 1 << ADSC;

            tick = 0;
        } else tick++;
*/
    }

    return 0;
}
