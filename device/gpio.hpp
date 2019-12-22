#pragma once

namespace device
{

struct GPIOC // General purpose I/O
{
    enum : unsigned // General purpose I/O
    {
        BASE = 0x40011000,
        CRL = BASE + 0x0,   // Port configuration register low (GPIOn_CRL)
        CRH = BASE + 0x4,   // Port configuration register high (GPIOn_CRL)
        IDR = BASE + 0x8,   // Port input data register (GPIOn_IDR)
        ODR = BASE + 0xC,   // Port output data register (GPIOn_ODR)
        BSRR = BASE + 0x10, // Port bit set/reset register (GPIOn_BSRR)
        BRR = BASE + 0x14,  // Port bit reset register (GPIOn_BRR)
        LCKR = BASE + 0x18, // Port configuration lock register
    };
};

} // namespace device
