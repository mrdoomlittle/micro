# include "wiring_digital.hpp"

int main(void)
{
    // turn on and off pin 8 of the microcontroller
    qg::set_pin_mode(DIGITAL_OUTPUT, 8);
    for (;;)
    {
        qg::set_pin_state(DIGITAL_HIGH, 8);
        _delay_ms(1000);
        qg::set_pin_state(DIGITAL_LOW, 8);
        _delay_ms(1000);

    }
}
