#include "system.h"

#include "module_config.h"
#include "module.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <unistd.h>

extern struct module *__module_list_begin[];
extern struct module *__module_list_end[];

static void AppTimerCallback( TimerHandle_t xTimer );
static void module_init(void);
static void SystemLEDBlink(void);

static void AppTimerCallback( TimerHandle_t xTimer )
{
    SystemLEDBlink();
}

static void module_init(void)
{
    int i;

    for (i = 0; i < (__module_list_end - __module_list_begin); i++) {
        struct module *m = __module_list_begin[i];
        if (m) {
            if (m->init) {
                m->init();
            }

            if (m->func) {
                xTaskCreateStatic(m->func, m->name, m->stack_depth, NULL, m->priority, m->stack, m->tcb);
            }
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

static void SystemLEDBlink(void)
{
    static char val = 0;

    write(MODULE_SYSTEM_BLINK1, &val, 1);

    write(MODULE_SYSTEM_BLINK2, &val, 1);

    val = !val;
}