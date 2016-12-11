# include "micro.hpp"

# include "serial_comm.hpp"

int main(void)
{
    _delay_ms(5000);

    serial_comm scom(9600, 0, 1);

    for (;;)
    {
        scom.transmit("Say Hello to the World! :O~");
        _delay_ms(1000);
    }

    return 0;
}
