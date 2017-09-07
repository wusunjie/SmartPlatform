#include "module.h"

#include "network.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>

MODULE_DEFINE(Test, 200, 1)
{
    uint16_t lac = 0, cid = 0;

    char *rsp = NULL;

    NetworkSetup(&lac, &cid);

    NetworkConnect("118.178.182.135", 8000);

    NetworkSubmit("Hello World\r\n", &rsp);

    NetworkShutdown();

    vTaskSuspend(NULL);
}

MODULE_INIT_DEFINE(Test)
{

}
