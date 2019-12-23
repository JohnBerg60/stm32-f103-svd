#pragma once

#include "drivers/register.hpp"
#include "device/systick.hpp"

using namespace device;

namespace drivers
{

class Systick : public Register
{
private:
    volatile static unsigned tickCount;

public:
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