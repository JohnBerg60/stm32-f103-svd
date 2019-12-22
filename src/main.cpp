#include "core/pin.hpp"
#include "drivers/gpio.hpp"

// delay loop for 8 MHz CPU clock with optimizer enabled
void delay(unsigned msec)
{
    for (unsigned j = 0; j < 2000UL * msec; j++)
    {
        asm("nop");
    }
}

int main()
{
    Pin<drivers::GPIOC, 13> led;

    while (true)
    {
        led.toggle();
        delay(200);
    }
}