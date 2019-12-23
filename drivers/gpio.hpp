#pragma once

#include "drivers/register.hpp"
#include "device/rcc.hpp"
#include "device/gpio.hpp"

namespace drivers
{

#define IOPCEN 1 << 4

struct Port : public Register
{
};

struct GPIOC : public Port, public device::GPIOC
{
    static void clockEnable()
    {
        setbits<device::RCC::APB2ENR>(IOPCEN);
    }
};

enum MODE : unsigned
{
    INPUT = 0b00,
    OUTPUT = 0b01,
    ALTERNATE = 0b10,
    ANALOG = 0b11
};

} // namespace drivers
