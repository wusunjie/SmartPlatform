#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"

int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();
	return 0;
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{

}

void vApplicationIdleHook( void )
{

}