#include "device.h"
#include "boardcfg.h"
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include <unistd.h>

extern uint32_t SystemCoreClock;

DEVICE_DEFINE(trace, STDOUT_FILENO);

DEVICE_FUNC_DEFINE_OPEN(trace)
{
    uint32_t SWOSpeed = 64000; /* default 64k baud rate */
    uint32_t SWOPrescaler = (SystemCoreClock / SWOSpeed) - 1; /* SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock */

    CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk; /* enable trace in core debug */
    *((volatile unsigned *)(ITM_BASE + 0x400F0)) = 0x00000002; /* "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO NRZ, 1: SWO Manchester encoding) */
    *((volatile unsigned *)(ITM_BASE + 0x40010)) = SWOPrescaler; /* "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output */
    *((volatile unsigned *)(ITM_BASE + 0x00FB0)) = 0xC5ACCE55; /* ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC */
    ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk; /* ITM Trace Control Register */
    ITM->TPR = ITM_TPR_PRIVMASK_Msk; /* ITM Trace Privilege Register */
    ITM->TER = 1; /* ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port. */
    *((volatile unsigned *)(ITM_BASE + 0x01000)) = 0x400003FE; /* DWT_CTRL */
    *((volatile unsigned *)(ITM_BASE + 0x40304)) = 0x00000100; /* Formatter and Flush Control Register */
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