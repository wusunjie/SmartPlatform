#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "boardcfg.h"
#include "module.h"
#include "network.h"

#include "task.h"
#include "event_groups.h"
#include "queue.h"

#define BIT_SETUP     ( 1 << 0 )
#define BIT_CONNECT   ( 1 << 1 )
#define BIT_SUBMIT    ( 1 << 2 )
#define BIT_SHUTDOWN  ( 1 << 3 )

enum NetworkStatus {
    NETWORK_STATUS_NREADY,
    NETWORK_STATUS_DISCONNECTED,
    NETWORK_STATUS_CONNECTED,
};

static enum NetworkStatus nstatus = NETWORK_STATUS_NREADY;

static QueueHandle_t LMQueue = NULL;

static EventGroupHandle_t LMEventGroup = NULL;

static uint16_t lac = 0, ci = 0;

static char addr[20] = {0};

static uint16_t port = 0;

static int doNetworkResult = -1;

static char rxbuf[1024] = {0};

#define GPS_MODULE_SEND(x) write(DEV_GPSCOM_ID, x, strlen(x))

static int doNetworkSetup(uint16_t *l, uint16_t *c);

static int doNetworkConnect(const char *a, uint16_t p);

static int doNetworkSubmit(const char *rq);

static int doNetworkShutdown(void);

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
        return doNetworkResult;
    }
    else {
        return -1;
    }
}

int NetworkConnect(const char *a, uint16_t p)
{
    uint16_t type = 1;

    EventBits_t uxBits;

    strcpy(addr, a);

    port = p;
    
    if( xQueueSendToBack( LMQueue, ( void * ) &type, ( TickType_t ) 10 ) != pdPASS ) {
        return -1;
    }

    uxBits = xEventGroupWaitBits(LMEventGroup, BIT_CONNECT, pdTRUE, pdTRUE, portMAX_DELAY);

    if( ( uxBits & BIT_CONNECT ) != 0 ) {
        return doNetworkResult;
    }
    else {
        return -1;
    }
}

int NetworkSubmit(const char *rq, char **rsp)
{
    uint16_t type = 2;

    EventBits_t uxBits;

    strcpy(rxbuf, rq);

    if( xQueueSendToBack( LMQueue, ( void * ) &type, ( TickType_t ) 10 ) != pdPASS ) {
        return -1;
    }

    uxBits = xEventGroupWaitBits(LMEventGroup, BIT_SUBMIT, pdTRUE, pdTRUE, portMAX_DELAY);

    if( ( uxBits & BIT_SUBMIT ) != 0 ) {
        *rsp = rxbuf;
        return doNetworkResult;
    }
    else {
        return -1;
    }
}

int NetworkShutdown(void)
{
    uint16_t type = 3;

    EventBits_t uxBits;

    if( xQueueSendToBack( LMQueue, ( void * ) &type, ( TickType_t ) 10 ) != pdPASS ) {
        return -1;
    }

    uxBits = xEventGroupWaitBits(LMEventGroup, BIT_SHUTDOWN, pdTRUE, pdTRUE, portMAX_DELAY);

    if( ( uxBits & BIT_SHUTDOWN ) != 0 ) {
        return doNetworkResult;
    }
    else {
        return -1;
    }
}

MODULE_DEFINE(Network, 1024, 1)
{
    uint16_t *pxRxedMessage;
    int val = 0;

    while (1) {

        write(DEV_LEDGPIO2_ID, &val, 1);

        if( xQueueReceive( LMQueue, &( pxRxedMessage ), ( TickType_t ) portMAX_DELAY ) ) {
            switch (*pxRxedMessage) {
                case 0:
                {
                    if (!doNetworkSetup(&lac, &ci)) {
                        doNetworkResult = 0;
                    }
                    else {
                        doNetworkResult = -1;
                    }

                    xEventGroupSetBits(LMEventGroup, BIT_SETUP);
                }
                break;
                case 1:
                {
                    if (!doNetworkConnect(addr, port)) {
                        doNetworkResult = 0;
                    }
                    else {
                        doNetworkResult = -1;
                    }

                    xEventGroupSetBits(LMEventGroup, BIT_CONNECT);
                }
                break;
                case 2:
                {
                    if (!doNetworkSubmit(rxbuf)) {
                        doNetworkResult = 0;
                    }
                    else {
                        doNetworkResult = -1;
                    }

                    xEventGroupSetBits(LMEventGroup, BIT_SUBMIT);
                }
                break;
                case 3:
                {
                    if (!doNetworkShutdown()) {
                        doNetworkResult = 0;
                    }
                    else {
                        doNetworkResult = -1;
                    }

                    xEventGroupSetBits(LMEventGroup, BIT_SHUTDOWN);
                }
                break;
                default:
                break;
            }
        }

        val = !val;
    }

    vTaskSuspend(NULL);
}

MODULE_INIT_DEFINE(Network)
{
    nstatus = NETWORK_STATUS_NREADY;
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
    static StaticQueue_t xQueueBuffer;

    // ucQueueStorage will hold the items posted to the queue.  Must be at least
    // [(queue length) * ( queue item size)] bytes long.
    static uint8_t ucQueueStorage[ 20 * sizeof(uint16_t) ];

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

    if (NETWORK_STATUS_NREADY == nstatus) {
        write(DEV_LEDGPIO1_ID, &val, 1);

        vTaskDelay(3000 / portTICK_PERIOD_MS);

        val = 0;
        write(DEV_LEDGPIO1_ID, &val, 1);

        GPS_MODULE_SEND("ATE0\r\n");

        while (1) {
            int ll = read(DEV_GPSCOM_ID, rxbuf + len, 1024);
            if (-1 != ll) {
                len += ll;
                if (strchr(rxbuf, '\r') || strchr(rxbuf, '\n')) {
                    switch (status) {
                        case 0:
                        {
                            if (strstr(rxbuf, "OK")) {
                                GPS_MODULE_SEND("AT+CCID\r\n");
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("ATE0\r\n");
                            }
                        }
                        break;
                        case 1:
                        {
                            if (strstr(rxbuf, "OK")) {
                                GPS_MODULE_SEND("AT+CREG=2\r\n");
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CCID\r\n");
                            }
                        }
                        break;
                        case 2:
                        {
                            if (strstr(rxbuf, "OK")) {
                                GPS_MODULE_SEND("AT+CREG?\r\n");
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CREG=2\r\n");
                            }
                        }
                        break;
                        case 3:
                        {
                            int n = -1, stat = -1;
                            int ret = sscanf(rxbuf, "%*[^+]+CREG: %d,%d,\"%hx\",\"%hx\"", &n, &stat, l, c);
                            if (ret >= 2) {
                                if (2 == n) {
                                    if (1 == stat) {
                                        GPS_MODULE_SEND("AT+CGATT=1\r\n");
                                        status++;
                                        break;
                                    }
                                }
                            }
                            if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CREG?\r\n");
                            }
                        }
                        break;
                        case 4:
                        {
                            if (strstr(rxbuf, "OK")) {
                                GPS_MODULE_SEND("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CGATT=1\r\n");
                            }
                        }
                        break;
                        case 5:
                        {
                            if (strstr(rxbuf, "OK")) {
                                GPS_MODULE_SEND("AT+CGACT=1,1\r\n");
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
                            }
                        }
                        break;
                        case 6:
                        {
                            if (strstr(rxbuf, "OK")) {
                                nstatus = NETWORK_STATUS_DISCONNECTED;
                                return 0;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CGACT=1,1\r\n");
                            }
                        }
                        break;
                        default:
                        break;
                    }
                    memset(rxbuf, 0, 1024);
                    len = 0;
                }
            }
        }
    }

    return -1;
}

static int doNetworkConnect(const char *a, uint16_t p)
{
    int len = 0;

    int status = 0;

    char strbuf[50] = {0};

    if (NETWORK_STATUS_DISCONNECTED == nstatus) {
        snprintf(strbuf, 50, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", a, p);

        GPS_MODULE_SEND(strbuf);

        while (1) {
            int ll = read(DEV_GPSCOM_ID, rxbuf + len, 1024);
            if (-1 != ll) {
                len += ll;
                if (strchr(rxbuf, '\r') || strchr(rxbuf, '\n')) {
                    switch (status) {
                        case 0:
                        {
                            if (strstr(rxbuf, "CONNECT OK")) {
                                GPS_MODULE_SEND("AT+CIPTMODE=1");
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND(strbuf);
                            }
                        }
                        break;
                        case 1:
                        {
                            if (strstr(rxbuf, "OK")) {
                                nstatus = NETWORK_STATUS_CONNECTED;
                                return 0;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPS_MODULE_SEND("AT+CIPTMODE=1");
                            }
                        }
                        break;
                        default:
                        break;
                    }
                    memset(rxbuf, 0, 1024);
                    len = 0;
                }
            }
        }
    }

    return -1;
}

static int doNetworkSubmit(const char *rq)
{
    if (NETWORK_STATUS_CONNECTED == nstatus) {
        GPS_MODULE_SEND(rq);
        return read(DEV_GPSCOM_ID, rxbuf, 1024);
    }
    else {
        return -1;
    }
}

static int doNetworkShutdown(void)
{
    if (NETWORK_STATUS_CONNECTED == nstatus) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        GPS_MODULE_SEND("+++");
        if (-1 != read(DEV_GPSCOM_ID, rxbuf, 1024)) {
            if (strstr(rxbuf, "OK")) {
                nstatus = NETWORK_STATUS_DISCONNECTED;
                return 0;
            }
        }
        return -1;
    }
    else {
        return 0;
    }
}