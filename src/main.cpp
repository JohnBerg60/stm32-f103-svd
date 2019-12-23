#include "core/pin.hpp"
#include "drivers/systick.hpp"
#include "drivers/gpio.hpp"

static void delay(unsigned ticks)
{
    unsigned ends = drivers::Systick::ticks() + ticks;
    while (drivers::Systick::ticks() < ends)
    {
    }
}

int main()
{
    drivers::Systick::init(8000000, 1000);
    Pin<drivers::GPIOC, 13> led;

    while (true)
    {
        led.toggle();
        delay(500);
    }
}