#include "device.h"
#include "boardcfg.h"
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include <unistd.h>

DEVICE_DEFINE(trace, STDOUT_FILENO);

DEVICE_FUNC_DEFINE_OPEN(trace)
{
    ITM->TCR |= ITM_TCR_ITMENA_Msk;
    ITM->TER |= 1UL;
}

DEVICE_FUNC_DEFINE_CLOSE(trace)
{
    ITM->TCR &= ~ITM_TCR_ITMENA_Msk;
    ITM->TER &= ~1UL;
}

DEVICE_FUNC_DEFINE_WRITE(trace)
{
    int i;

    for (i = 0; i < len; i++) {
        ITM_SendChar(buf[i]);
    }

    return len;
}

DEVICE_FUNC_DEFINE_READ(trace)
{
    return -1;
}