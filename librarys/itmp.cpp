# define RX_PID 6
# define TX_PID 7

// external clock
# define CL_PID 8
namespace itmp {

void init() {
    set_pin_mode(DIGITAL_INPUT, RX_PID);
    set_pin_mode(DIGITAL_OUTPUT, TX_PID);
    set_pin_mode(DIGITAL_INPUT, CL_PID);
}

// 00000000
static char o = '\0';
static char i = '\0';
void send(uint8_t l) {
    uint8_t b = 0; 
    b = (o >> l) & 1;
    set_pin_state(b == 1? DIGITAL_HIGH : DIGITAL_LOW, TX_PID);
}

void recv(uint8_t l) {
    uint8_t re = get_pin_state(RX_PID);
    i |= (re == DIGITAL_HIGH? 1 : 0) << l; 
}

void tick() {
    for (;;) {
        if (get_pin_state(CL_PID) == DIGITAL_HIGH) {
            for (uint8_t l = 0; l != 8; l ++) {
                send(l);
                recv(l);
                _delay_ms(500);
            }
            break;
        }
    }
}

}
