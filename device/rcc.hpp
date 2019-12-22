#pragma once

struct RCC // Reset and clock control
{
    enum : unsigned // Reset and clock control
    {
        BASE = 0x40021000,
        CR = BASE + 0x0,        // Clock control register
        CFGR = BASE + 0x4,      // Clock configuration register (RCC_CFGR)
        CIR = BASE + 0x8,       // Clock interrupt register (RCC_CIR)
        APB2RSTR = BASE + 0xC,  // APB2 peripheral reset register (RCC_APB2RSTR)
        APB1RSTR = BASE + 0x10, // APB1 peripheral reset register (RCC_APB1RSTR)
        AHBENR = BASE + 0x14,   // AHB Peripheral Clock enable register (RCC_AHBENR)
        APB2ENR = BASE + 0x18,  // APB2 peripheral clock enable register (RCC_APB2ENR)
        APB1ENR = BASE + 0x1C,  // APB1 peripheral clock enable register (RCC_APB1ENR)
        BDCR = BASE + 0x20,     // Backup domain control register (RCC_BDCR)
        CSR = BASE + 0x24,      // Control/status register (RCC_CSR)
    };
};