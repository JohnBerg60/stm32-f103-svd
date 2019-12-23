#include "drivers/systick.hpp"

volatile unsigned int tickCount = 0;

#ifdef __cplusplus
extern "C"
{
#endif

    void SysTick_Handler(void)
    {
        tickCount++;
    }

#ifdef __cplusplus
}
#endif