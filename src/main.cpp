#include "gpio.hpp"

using namespace GPIO;

int main()
{
    Pin<GPIOC, 13> led;

    while (true)
    {
        led.set();
        led.clear();
    }
}