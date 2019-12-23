#pragma once

#include "drivers/register.hpp"
#include "device/systick.hpp"

extern volatile unsigned int tickCount;

using namespace device;

namespace drivers
{

struct Systick : public Register
{
    static void init(int HCLKFrequency, int Ticks)
    {
        regwrite<SYSTICK::LOAD>(HCLKFrequency / Ticks - 1UL);
        regwrite<SYSTICK::VAL>(0);
        setbits<SYSTICK::CTRL>((1 << SYSTICK::CLKSOURCE) + (1 << SYSTICK::TICKINT));
        enable();
    }

    static void enable()
    {
        setbits<SYSTICK::CTRL>(1 << SYSTICK::ENABLE);
    }

    static void tick()
    {
        tickCount++;
    }

    static unsigned ticks()
    {
        return tickCount;
    }
};

} // namespace drivers