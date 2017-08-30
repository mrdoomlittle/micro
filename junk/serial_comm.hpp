# ifndef __serial__comm__hpp__
# define __serial__comm__hpp__
/*
NOTE: ignore this file and its code as its only for testing/experimenting

*/

# include <stddef.h>
size_t get_strlen(const char * __str)
{
    size_t length_of_str = 0;
    while (*__str != '\0') {
        __str ++;
        length_of_str ++;
    }
    return length_of_str;
}

class serial_comm
{
    public:
    serial_comm(uint16_t __baudrate, uint8_t __rxpid, uint8_t __txpid)
    : sinfo(__baudrate, __rxpid, __txpid)
    {
        set_pin_mode(DIGITAL_OUTPUT, __txpid);
        //set_pin_state(DIGITAL_LOW, __txpid);
        set_pin_mode(DIGITAL_INPUT, __rxpid);
    }

    void transmit_byte(uint8_t __byte) {
        set_pin_state(DIGITAL_LOW, sinfo.txpid); 
        _delay_us(sinfo.delay);


        for (uint8_t i = 0; i != 8; i ++) {
            uint8_t bit_state = 0;
            bit_state = ((__byte >> i) & 1) == 1? DIGITAL_HIGH : DIGITAL_LOW;
            set_pin_state(bit_state, sinfo.txpid);
            _delay_us(sinfo.delay);
        } 

        set_pin_state(DIGITAL_HIGH, sinfo.txpid);
        _delay_us(sinfo.delay);
    }

    void transmit(char const * __data, uint16_t __size) {
        for (uint8_t i = 0; i != __size; i ++)
            transmit_byte(__data[i]);
    }

    void transmit(char const * __data) {
        transmit(__data, get_strlen(__data));
    }

    private:
    struct serial_info {
        serial_info(uint16_t __baudrate, uint8_t __rxpid, uint8_t __txpid)
        : baudrate(__baudrate),  rxpid(__rxpid), txpid(__txpid), delay(((16000000/16/__baudrate) - 1)) {}
      
        uint16_t const baudrate;
        uint16_t const delay;
        uint8_t const rxpid, txpid;
    };

    serial_info sinfo;
} ;

# endif /*__serial__comm__hpp__*/
