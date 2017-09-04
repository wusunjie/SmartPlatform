#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "boardcfg.h"
#include "module.h"

#include "task.h"

#define GPS_MODULE_SEND(x) write(DEV_GPSCOM_ID, x, strlen(x))

static char buf[1024] = {0}; /* reduce the stack usage. */

MODULE_DEFINE(Location, 1024, 1)
{
    int len = 0;

    char val = 1;

    int status = 0;

    uint16_t lac = 0, ci = 0;

    write(DEV_LEDGPIO1_ID, &val, 1);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    val = 0;
    write(DEV_LEDGPIO1_ID, &val, 1);

    GPS_MODULE_SEND("ATE0\r\n");

    while (1) {
        uint16_t l = read(DEV_GPSCOM_ID, buf + len, 1024);
        /* print log via ITM port 0 */
        printf("%s", buf);
        if (-1 != l) {
            len += l;
            if (strchr(buf, '\r') || strchr(buf, '\n')) {
                switch (status) {
                    case 0:
                    {
                        if (strstr(buf, "OK")) {
                            GPS_MODULE_SEND("AT+CCID\r\n");
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("ATE0\r\n");
                        }
                    }
                    break;
                    case 1:
                    {
                        if (strstr(buf, "OK")) {
                            GPS_MODULE_SEND("AT+CREG=2\r\n");
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CCID\r\n");
                        }
                    }
                    break;
                    case 2:
                    {
                        if (strstr(buf, "OK")) {
                            GPS_MODULE_SEND("AT+CREG?\r\n");
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CREG=2\r\n");
                        }
                    }
                    break;
                    case 3:
                    {
                        int n = -1, stat = -1;
                        int ret = sscanf(buf, "%*[^+]+CREG: %d,%d,\"%hx\",\"%hx\"", &n, &stat, &lac, &ci);
                        if (ret >= 2) {
                            if (2 == n) {
                                if (1 == stat) {
                                    GPS_MODULE_SEND("AT+CGATT=1\r\n");
                                    status++;
                                    break;
                                }
                            }
                        }
                        if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CREG?\r\n");
                        }
                    }
                    break;
                    case 4:
                    {
                        if (strstr(buf, "OK")) {
                            GPS_MODULE_SEND("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CGATT=1\r\n");
                        }
                    }
                    break;
                    case 5:
                    {
                        if (strstr(buf, "OK")) {
                            GPS_MODULE_SEND("AT+CGACT=1,1\r\n");
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
                        }
                    }
                    break;
                    case 6:
                    {
                        if (strstr(buf, "OK")) {
                            GPS_MODULE_SEND("AT+CIPSTART=\"TCP\",\"202.108.35.235\",80");
                            val = !val;
                            write(DEV_LEDGPIO2_ID, &val, 1);
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CGACT=1,1\r\n");
                        }
                    }
                    break;
                    case 7:
                    {
                        if (strstr(buf, "CONNECT OK")) {
                            GPS_MODULE_SEND("AT+CIPTMODE=1");
                            val = !val;
                            write(DEV_LEDGPIO2_ID, &val, 1);
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CIPSTART=\"TCP\",\"120.76.213.49\",48758");
                        }
                    }
                    break;
                    case 8:
                    {
                        if (strstr(buf, "OK")) {
                            val = !val;
                            write(DEV_LEDGPIO2_ID, &val, 1);
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CIPTMODE=1");
                        }
                    }
                    break;
                    default:
                    break;
                }
                memset(buf, 0, 1024);
                len = 0;
            }
        }
    }

    vTaskSuspend(NULL);
}
