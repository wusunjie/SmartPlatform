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

    NetworkConnect("202.108.35.250", 80);

    NetworkSubmit("GET /cellid.php?lac=6214&cell_id=54290&token=0d5de1203d28ed4e87cdf2e38224fe62 HTTP/1.1\r\nHost: api.cellid.cn\r\n\r\n", &rsp);

    NetworkShutdown();

    vTaskSuspend(NULL);
}

MODULE_INIT_DEFINE(Test)
{

}
