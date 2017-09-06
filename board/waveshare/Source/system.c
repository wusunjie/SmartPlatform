#include "stm32f4xx_hal.h"

void system_init(void)
{
    SystemCoreClockUpdate();
    HAL_Init();
}

void system_tick(void)
{
    HAL_IncTick();
}