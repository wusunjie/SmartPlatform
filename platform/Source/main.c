#include "system.h"

#include "module_config.h"
#include "module.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <unistd.h>

extern struct module *__module_list_begin[];
extern struct module *__module_list_end[];

static void module_init(void);

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
    system_init();

    module_init();

    vTaskStartScheduler();

    while (1);

    return 0;
}
