#ifndef _SMART_PLATFORM_LOCATION_H
#define _SMART_PLATFORM_LOCATION_H

#include <stdint.h>

extern int NetworkSetup(uint16_t *l, uint16_t *c);

extern int NetworkConnect(const char *a, uint16_t p);

#endif