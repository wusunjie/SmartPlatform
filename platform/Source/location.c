#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "boardcfg.h"
#include "module.h"
#include "location.h"

#include "task.h"
#include "event_groups.h"
#include "queue.h"

#define BIT_SETUP     ( 1 << 0 )
#define BIT_CONNECT   ( 1 << 1 )

static QueueHandle_t LMQueue = NULL;

static EventGroupHandle_t LMEventGroup = NULL;

static uint16_t lac = 0, ci = 0;

static char addr[20] = {0};

static uint16_t port = 0;

static int doNetworkSetupResult = -1;

static int doNetworConnectResult = -1;

static char buf[1024] = {0}; /* reduce the stack usage. */

#define GPS_MODULE_SEND(x) write(DEV_GPSCOM_ID, x, strlen(x))

static int doNetworkSetup(uint16_t *l, uint16_t *c);

static int doNetworkConnect(const char *a, uint16_t p);

int NetworkSetup(uint16_t *l, uint16_t *c)
{
    uint16_t type = 0;

    EventBits_t uxBits;
    
    if( xQueueSendToBack( LMQueue, ( void * ) &type, ( TickType_t ) 10 ) != pdPASS ) {
        return -1;
    }

    uxBits = xEventGroupWaitBits(LMEventGroup, BIT_SETUP, pdTRUE, pdTRUE, portMAX_DELAY);

    if( ( uxBits & BIT_SETUP ) != 0 ) {
        *l = lac;
        *c = ci;
        return doNetworkSetupResult;
    }
    else {
        return -1;
    }
}

int NetworkConnect(const char *a, uint16_t p)
{
    uint16_t type = 0;

    EventBits_t uxBits;

    strcpy(addr, a);

    port = p;
    
    if( xQueueSendToBack( LMQueue, ( void * ) &type, ( TickType_t ) 10 ) != pdPASS ) {
        return -1;
    }

    uxBits = xEventGroupWaitBits(LMEventGroup, BIT_CONNECT, pdTRUE, pdTRUE, portMAX_DELAY);

    if( ( uxBits & BIT_CONNECT ) != 0 ) {
        return doNetworkSetupResult;
    }
    else {
        return -1;
    }
}

MODULE_DEFINE(Location, 1024, 1)
{
    uint16_t *pxRxedMessage;

    while (1) {
        if( xQueueReceive( LMQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) ) {
            switch (*pxRxedMessage) {
                case 0:
                {
                    if (doNetworkSetup(&lac, &ci)) {
                        doNetworkSetupResult = 0;
                    }
                    else {
                        doNetworkSetupResult = -1;
                    }

                    xEventGroupSetBits(LMEventGroup, BIT_SETUP);
                }
                break;
                case 1:
                {
                    if (doNetworkConnect(addr, port)) {
                        doNetworConnectResult = 0;
                    }
                    else {
                        doNetworConnectResult = -1;
                    }

                    xEventGroupSetBits(LMEventGroup, BIT_CONNECT);
                }
                break;
                default:
                break;
            }
        }
    }

    vTaskSuspend(NULL);
}

MODULE_INIT_DEFINE(Location)
{
    // StaticEventGroup_t is a publicly accessible structure that has the same
    // size and alignment requirements as the real event group structure.  It is
    // provided as a mechanism for applications to know the size of the event
    // group (which is dependent on the architecture and configuration file
    // settings) without breaking the strict data hiding policy by exposing the
    // real event group internals.  This StaticEventGroup_t variable is passed
    // into the xSemaphoreCreateEventGroupStatic() function and is used to store
    // the event group's data structures
    static StaticEventGroup_t xEventGroupBuffer;

    // Create the event group without dynamically allocating any memory.
    LMEventGroup = xEventGroupCreateStatic( &xEventGroupBuffer );

    // xQueueBuffer will hold the queue structure.
    StaticQueue_t xQueueBuffer;

    // ucQueueStorage will hold the items posted to the queue.  Must be at least
    // [(queue length) * ( queue item size)] bytes long.
    uint8_t ucQueueStorage[ 20 * sizeof(uint16_t) ];

    LMQueue = xQueueCreateStatic( 20, // The number of items the queue can hold.
                            sizeof(uint16_t),     // The size of each item in the queue
                            &( ucQueueStorage[ 0 ] ), // The buffer that will hold the items in the queue.
                            &xQueueBuffer ); // The buffer that will hold the queue structure.
}

static int doNetworkSetup(uint16_t *l, uint16_t *c)
{
    int len = 0;

    char val = 1;

    int status = 0;

    write(DEV_LEDGPIO1_ID, &val, 1);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    val = 0;
    write(DEV_LEDGPIO1_ID, &val, 1);

    GPS_MODULE_SEND("ATE0\r\n");

    while (1) {
        uint16_t l = read(DEV_GPSCOM_ID, buf + len, 1024);
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
                        int ret = sscanf(buf, "%*[^+]+CREG: %d,%d,\"%hx\",\"%hx\"", &n, &stat, l, c);
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
                            
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
                        }
                    }
                    break;
                    case 6:
                    {
                        if (strstr(buf, "OK")) {
                            val = !val;
                            write(DEV_LEDGPIO2_ID, &val, 1);
                            return 0;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CGACT=1,1\r\n");
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

    return -1;
}

static int doNetworkConnect(const char *a, uint16_t p)
{
    int len = 0;

    char val = 1;

    int status = 0;

    char strbuf[50] = {0};

    snprintf(strbuf, 50, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", a, p);

    GPS_MODULE_SEND(strbuf);

    while (1) {
        uint16_t l = read(DEV_GPSCOM_ID, buf + len, 1024);
        if (-1 != l) {
            len += l;
            if (strchr(buf, '\r') || strchr(buf, '\n')) {
                switch (status) {
                    case 0:
                    {
                        if (strstr(buf, "CONNECT OK")) {
                            GPS_MODULE_SEND("AT+CIPTMODE=1");
                            val = !val;
                            write(DEV_LEDGPIO2_ID, &val, 1);
                            status++;
                        }
                        else if (strstr(buf, "ERROR")) {
                            GPS_MODULE_SEND("AT+CIPSTART=\"TCP\",\"120.76.213.49\",56012\r\n");
                        }
                    }
                    break;
                    case 1:
                    {
                        if (strstr(buf, "OK")) {
                            val = !val;
                            write(DEV_LEDGPIO2_ID, &val, 1);
                            return 0;
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

    return -1;
}

