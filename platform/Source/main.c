#include "system.h"

#include "boardcfg.h"
#include "module.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <unistd.h>

extern struct module *__module_list_begin[];
extern struct module *__module_list_end[];

static void AppTimerCallback( TimerHandle_t xTimer );
static void module_init(void);

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

static void module_init(void)
{
    int i;

    for (i = 0; i < (__module_list_end - __module_list_begin); i++) {
        struct module *m = __module_list_begin[i];
        if (m) {
            xTaskCreateStatic(m->func, m->name, m->stack_depth, NULL, m->priority, m->stack, m->tcb);
        }
    }
}

int main(void)
{
    static TimerHandle_t timer;

    static StaticTimer_t BlinkTimer;

    system_init();

    module_init();

    timer = xTimerCreateStatic("BlinkTimer", 1000 / portTICK_PERIOD_MS, pdTRUE, NULL, AppTimerCallback, &BlinkTimer);

    if (timer) {
        xTimerStart(timer, 0);
        vTaskStartScheduler();
    }

    while (1);

    return 0;
}
