#ifndef _SMART_PLATFORM_MODULE_H
#define _SMART_PLATFORM_MODULE_H

#include "FreeRTOS.h"

typedef void (*PlatformModuleTask)( void *args );

struct module {
    const PlatformModuleTask func;
    const char *name;
    const uint32_t stack_depth;
    const UBaseType_t priority;
    StaticTask_t *tcb;
    StackType_t *stack;
};

#define MODULE_DEFINE(n, d, p)                                               \
    static void Platform##n##Task( void *args );                             \
    static StaticTask_t n##TaskBuffer;                                       \
    static StackType_t n##Stack[d];                                          \
    static struct module n##_Module = {                                      \
        .func = Platform##n##Task,                                           \
        .name = #n,                                                          \
        .stack_depth = d,                                                    \
        .priority = p,                                                       \
        .tcb = &n##TaskBuffer,                                               \
        .stack = n##Stack                                                    \
    };                                                                       \
    static const struct module *n##_ModulePlatform __attribute__((__used__)) \
    __attribute__((section(".module"))) = &n##_Module;                       \
    static void Platform##n##Task( void *args )

#endif
