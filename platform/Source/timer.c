#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"

void vApplicationTickHook( void )
{
    HAL_IncTick();
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{

}