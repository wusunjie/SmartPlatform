#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "module_config.h"
#include "module.h"
#include "network.h"

#include "task.h"
#include "event_groups.h"
#include "queue.h"

#define RETRY_MAX     3

#define BIT_SETUP     ( 1 << 0 )
#define BIT_CONNECT   ( 1 << 1 )
#define BIT_SUBMIT    ( 1 << 2 )
#define BIT_SHUTDOWN  ( 1 << 3 )

enum NetworkStatus {
    NETWORK_STATUS_NREADY,
    NETWORK_STATUS_DISCONNECTED,
    NETWORK_STATUS_CONNECTED,
};

struct gprscmd {
    const char *cmd;
    uint16_t len;
    const char *rsp;
    uint32_t timeout;
};

struct dialchart {
    const char *abort[10];
    struct gprscmd *cmdlist;
    uint16_t len;
    uint16_t retry;
};

static struct gprscmd cmds[] = {
    {
        .cmd = "AT\r\n",
        .len = sizeof("AT\r\n") - 1,
        .rsp = "OK",
        .timeout = 3000
    },
    {
        .cmd = "ATE0\r\n",
        .len = sizeof("ATE0\r\n") - 1,
        .rsp = "OK",
        .timeout = 3000
    },
    {
        .cmd = "AT+CPIN?\r\n",
        .len = sizeof("AT+CPIN?\r\n") - 1,
        .rsp = "CPIN: READY",
        .timeout = 3000
    },
    {
        .cmd = "AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n",
        .len = sizeof("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n") - 1,
        .rsp = "OK",
        .timeout = 3000
    },
    {
        .cmd = "AT+CGATT=1\r\n",
        .len = sizeof("AT+CGATT=1\r\n") - 1,
        .rsp = "OK",
        .timeout = 3000
    },
    {
        .cmd = "AT+CGACT=1,1\r\n",
        .len = sizeof("AT+CGACT=1,1\r\n") - 1,
        .rsp = "OK",
        .timeout = 3000
    }
};

static struct dialchart dials = {
    .abort = {
        "NO CARRIER",
        "ERROR",
        "NO DIALTONE",
        "BUSY",
        "NO ANSWER",
        NULL
    },
    .cmdlist = cmds,
    .len = sizeof(cmds),
    .retry = 3
};

static enum NetworkStatus nstatus = NETWORK_STATUS_NREADY;

static QueueHandle_t LMQueue = NULL;

static EventGroupHandle_t LMEventGroup = NULL;

static char addr[20] = {0};

static uint16_t port = 0;

static int doNetworkResult = -1;

static char rxbuf[1024] = {0};

#define GPRS_MODULE_SEND(x, y) write(MODULE_NETWORK_DEV, x, y)

#define GPRS_MODULE_RECV(x) read(MODULE_NETWORK_DEV, x, 1024)

static int doNetworkSetup(void);

static int doNetworkConnect(const char *a, uint16_t p);

static int doNetworkSubmit(const char *rq);

static int doNetworkShutdown(void);

static void GPRS_ModulePwron(void);

static void GPRS_ModuleBlink(void);

int NetworkSetup(void)
{
    uint16_t type = 0;

    EventBits_t uxBits;
    
    if( xQueueSendToBack( LMQueue, ( void * ) &type, ( TickType_t ) 10 ) != pdPASS ) {
        return -1;
    }

    uxBits = xEventGroupWaitBits(LMEventGroup, BIT_SETUP, pdTRUE, pdTRUE, portMAX_DELAY);

    if( ( uxBits & BIT_SETUP ) != 0 ) {
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

MODULE_DEFINE(Network, 1024, 2)
{
    uint16_t pxRxedMessage;

    while (1) {

        GPRS_ModuleBlink();

        if( xQueueReceive( LMQueue, &( pxRxedMessage ), ( TickType_t ) portMAX_DELAY ) ) {
            switch (pxRxedMessage) {
                case 0:
                {
                    doNetworkResult = doNetworkSetup();

                    xEventGroupSetBits(LMEventGroup, BIT_SETUP);
                }
                break;
                case 1:
                {
                    doNetworkResult = doNetworkConnect(addr, port);

                    xEventGroupSetBits(LMEventGroup, BIT_CONNECT);
                }
                break;
                case 2:
                {
                    doNetworkResult = doNetworkSubmit(rxbuf);

                    xEventGroupSetBits(LMEventGroup, BIT_SUBMIT);
                }
                break;
                case 3:
                {
                    doNetworkResult = doNetworkShutdown();

                    xEventGroupSetBits(LMEventGroup, BIT_SHUTDOWN);
                }
                break;
                default:
                break;
            }
        }
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
    static uint8_t ucQueueStorage[ sizeof(uint16_t) ];

    LMQueue = xQueueCreateStatic( 1, // The number of items the queue can hold.
                            sizeof(uint16_t),     // The size of each item in the queue
                            &( ucQueueStorage[ 0 ] ), // The buffer that will hold the items in the queue.
                            &xQueueBuffer ); // The buffer that will hold the queue structure.
}

static int doNetworkSetup(void)
{
    if (NETWORK_STATUS_NREADY == nstatus) {
        int len = 0;
        int ll = -1;
        int status = 0;

        memset(rxbuf, 0, 1024);

        while (1) {
            int i;

            vTaskDelay(dials.cmdlist[status].timeout / portTICK_PERIOD_MS);

            GPRS_MODULE_SEND(dials.cmdlist[status].cmd, dials.cmdlist[status].len);

            ll = GPRS_MODULE_RECV(rxbuf + len);
            if (-1 == ll) {
                continue;
            }

            for (i = 0; NULL != dials.abort[i]; i++) {
                if (strstr(rxbuf, dials.abort[i])) {
                    return -1;
                }
            }

            if (!strstr(rxbuf, dials.cmdlist[status].rsp)) {
                return -1;
            }

            if (status + 1 == dials.len) {
                return 0;
            }
            else {
                memset(rxbuf, 0, 1024);
                status++;
            }
        }
    }

    if (NETWORK_STATUS_NREADY == nstatus) {
        return -1;
    }

    return 0;
}

static int doNetworkConnect(const char *a, uint16_t p)
{
    if (NETWORK_STATUS_DISCONNECTED == nstatus) {

        int len = 0;

        int status = 0;

        static char strbuf[50] = {0};

        uint16_t retry = 0;

        int tmpstatus = status;

        snprintf(strbuf, 50, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", a, p);

        GPRS_MODULE_SEND(strbuf, strlen(strbuf));

        while (1) {
            int ll = GPRS_MODULE_RECV(rxbuf + len);
            if (-1 != ll) {
                tmpstatus = status;
                len += ll;
                if (strchr(rxbuf, '\r') || strchr(rxbuf, '\n')) {
                    switch (status) {
                        case 0:
                        {
                            if (strstr(rxbuf, "CONNECT OK")) {
                                GPRS_MODULE_SEND("AT+CIPTMODE=1\r\n", sizeof("AT+CIPTMODE=1\r\n") - 1);
                                status++;
                            }
                            else if (strstr(rxbuf, "ERROR")) {
                                GPRS_MODULE_SEND(strbuf, strlen(strbuf));
                                retry++;
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
                                GPRS_MODULE_SEND("AT+CIPTMODE=1\r\n", sizeof("AT+CIPTMODE=1\r\n") - 1);
                                retry++;
                            }
                        }
                        break;
                        default:
                        break;
                    }
                    memset(rxbuf, 0, 1024);
                    len = 0;
                }

                if (tmpstatus < status) {
                    retry = 0;
                }
                else if (retry > RETRY_MAX) {
                    break;
                }
            }
        }
    }

    if (NETWORK_STATUS_CONNECTED == nstatus) {
        return 0;
    }

    return -1;
}

static int doNetworkSubmit(const char *rq)
{
    if (NETWORK_STATUS_CONNECTED == nstatus) {
        int ret = 0;
        GPRS_MODULE_SEND(rq, strlen(rq));
        ret = GPRS_MODULE_RECV(rxbuf);
        if (-1 != ret) {
            rxbuf[ret] = '\0';
        }
        return ret;
    }
    else {
        return -1;
    }
}

static int doNetworkShutdown(void)
{
    if (NETWORK_STATUS_CONNECTED == nstatus) {

        vTaskDelay(3000 / portTICK_PERIOD_MS);

        GPRS_MODULE_SEND("+++", sizeof("+++") - 1);

        if (-1 != GPRS_MODULE_RECV(rxbuf)) {
            if (strstr(rxbuf, "OK")) {
                GPRS_MODULE_SEND("AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n") - 1);
                /* should not wait the reply:
                   now we can't detect the disconnect from server.

                   1. if now is connected, AT+CIPCLOSE\r\n works well.
                   2. if now is disconnected, module is ready to accept the next command. */
            }
        }
    }

    if (NETWORK_STATUS_NREADY != nstatus) {
        nstatus = NETWORK_STATUS_DISCONNECTED;
    }

    return 0;
}

static void GPRS_ModulePwron(void)
{
    char val = 1;

    write(MODULE_NETWORK_PWR, &val, 1);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    val = 0;
    write(MODULE_NETWORK_PWR, &val, 1);
}

static void GPRS_ModuleBlink(void)
{
    static int val = 0;
    write(MODULE_NETWORK_BLINK, &val, 1);
    val = !val;
}
