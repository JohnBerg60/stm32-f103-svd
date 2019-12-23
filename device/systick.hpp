#pragma once

namespace device
{

struct SYSTICK
{
    enum : unsigned
    {
        BASE = 0xE000E010,  /*!< (@ 0xE000E010) STK Structure                                              */
        CTRL = BASE,        /*!< (@ 0x00000000) SysTick control and status register                        */
        LOAD = BASE + 0x04, /*!< (@ 0x00000004) SysTick reload value register                              */
        VAL = BASE + 0x08,  /*!< (@ 0x00000008) SysTick current value register                             */
        CALIB = BASE + 0x0C /*!< (@ 0x0000000C) SysTick calibration value register                         */
    };

    enum : unsigned
    {
        ENABLE = 0,
        TICKINT = 1,
        CLKSOURCE = 2
    };
};

} // namespace device