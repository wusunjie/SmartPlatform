#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "boardcfg.h"
#include "module.h"

#define GPS_MODULE_SEND(x) write(DEV_GPSCOM_ID, x, sizeof(x) - 1)

static char buf[1024] = {0}; /* reduce the stack usage. */

MODULE_DEFINE(Location, 200, 1)
{
    int len = 0;

    int status = 0;

    uint16_t lac = 0, ci = 0;

    while (1) {
        uint16_t l = 0;

        switch (status) {
            case 0:
            {
                GPS_MODULE_SEND("AT\r\n");
            }
            break;
            case 1:
            {
                GPS_MODULE_SEND("ATE0\r\n");
            }
            break;
            case 2:
            {
                GPS_MODULE_SEND("AT+CCID\r\n");
            }
            break;
            case 3:
            {
                GPS_MODULE_SEND("AT+CREG?\r\n");
            }
            break;
            case 4:
            {
                GPS_MODULE_SEND("AT+CGATT=1\r\n");
            }
            break;
            case 5:
            {
                GPS_MODULE_SEND("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
            }
            break;
            case 6:
            {
                GPS_MODULE_SEND("AT+CGACT=1,1");
            }
            break;
            case 7:
            {
                GPS_MODULE_SEND("AT+CIPSTART=\"TCP\",\"202.108.35.235\",80");
            }
            break;
            case 8:
            {
                GPS_MODULE_SEND("AT+CIPTMODE=1");
            }
            break;
            default:
            break;
        }
        l = read(DEV_GPSCOM_ID, buf + len, 1024);
        if (-1 != l) {
            len += l;
            if (strchr(buf, '\r') || strchr(buf, '\n')) {
                switch (status) {
                    case 0:
                    {
                        if (strstr(buf, "OK")) {
                            status = 1;
                        }
                    }
                    break;
                    case 1:
                    {
                        if (strstr(buf, "OK")) {
                            status = 2;
                        }
                    }
                    break;
                    case 2:
                    {
                        if (strstr(buf, "OK")) {
                            status = 3;
                        }
                    }
                    break;
                    case 3:
                    {
                        int n = -1, stat = -1;
                        int ret = sscanf(buf, "+CREG: %d,%d,%hx,%hx", &n, &stat, &lac, &ci);
                        if (ret >= 2) {
                            if (2 == n) {
                                if (1 == stat) {
                                    status = 4;
                                }
                            }
                        }
                    }
                    break;
                    case 4:
                    {
                        if (strstr(buf, "OK")) {
                            status = 5;
                        }
                    }
                    break;
                    case 5:
                    {
                        if (strstr(buf, "OK")) {
                            status = 6;
                        }
                    }
                    break;
                    case 6:
                    {
                        if (strstr(buf, "OK")) {
                            status = 7;
                        }
                    }
                    break;
                    case 7:
                    {
                        if (strstr(buf, "CONNECT OK")) {
                            status = 8;
                        }
                    }
                    break;
                    case 8:
                    {
                        if (strstr(buf, "OK")) {
                            status = 9;
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
