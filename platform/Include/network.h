#ifndef _SMART_PLATFORM_NETWORK_H
#define _SMART_PLATFORM_NETWORK_H

#include <stdint.h>

extern int NetworkSetup(void);

extern int NetworkConnect(const char *a, uint16_t p);

extern int NetworkSubmit(const char *rq, char **rsp);

extern int NetworkShutdown(void);

#endif
