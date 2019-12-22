#include "syscfg.h"

void SysTick_Handler(void)
{
    systick_count++;
}
