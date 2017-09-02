#include "system.h"

#include "boardcfg.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <string.h>

#include <unistd.h>

static char buf[1024] = {0}; /* reduce the stack usage. */

static void AppTimerCallback( TimerHandle_t xTimer );
static void PlatformSetupTask( void *args );

static void AppTimerCallback( TimerHandle_t xTimer )
{
    char val = 0;

    if (read(DEV_LEDGPIO3_ID, &val, 1)) {
        val = !val;
        write(DEV_LEDGPIO3_ID, &val, 1);
    }
    if (read(DEV_LEDGPIO4_ID, &val, 1)) {
        val = !val;
        write(DEV_LEDGPIO4_ID, &val, 1);
    }
}

static TimerHandle_t timer;

int main(void)
{
    static StaticTimer_t BlinkTimer;

    /* Structure that will hold the TCB of the task being created. */
    static StaticTask_t xTaskBuffer;

    /* Buffer that the task being created will use as its stack.  Note this is
    an array of StackType_t variables.  The size of StackType_t is dependent on
    the RTOS port. */
    static StackType_t xStack[ 200 ];

    char val = 1;

    system_init();

    write(DEV_LEDGPIO1_ID, &val, 1);
    write(DEV_LEDGPIO2_ID, &val, 1);

    xTaskCreateStatic(PlatformSetupTask, "PlatformSetup", 200, NULL, 1, xStack, &xTaskBuffer);

    timer = xTimerCreateStatic("BlinkTimer", 1000 / portTICK_PERIOD_MS, pdTRUE, NULL, AppTimerCallback, &BlinkTimer);

    if (timer) {
        vTaskStartScheduler();
    }

    while (1);

    return 0;
}

void PlatformSetupTask( void *args )
{
    int val = 1;
    int len = 0;

    write(DEV_LEDGPIO3_ID, &val, 1);
    write(DEV_LEDGPIO4_ID, &val, 1);

    xTimerStart(timer, 0);

    while (1) {
        uint16_t l = read(DEV_GPSCOM_ID, buf + len, 1024);
        if (-1 != l) {
            len += l;
            if (strchr(buf, '\r') || strchr(buf, '\n')) {
                write(DEV_GPSCOM_ID, buf, len);
                memset(buf, 0, 1024);
                len = 0;
            }
        }
    }

    vTaskSuspend(NULL);
}
