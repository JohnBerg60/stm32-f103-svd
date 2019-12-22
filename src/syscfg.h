#ifndef SYSCFG_H
#define SYSCFG_H

#include <stdint.h>

extern uint32_t SystemCoreClock;
extern volatile uint32_t systick_count;

#define on true
#define off false

#endif