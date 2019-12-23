#include "drivers/systick.hpp"

unsigned volatile drivers::Systick::tickCount = 0;

#ifdef __cplusplus
extern "C"
{
#endif

    void SysTick_Handler(void)
    {
        drivers::Systick::tick();
    }

#ifdef __cplusplus
}
#endif