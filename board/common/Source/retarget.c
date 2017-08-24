#include <stdlib.h>

#include "device.h"

extern struct device **__device_list;
extern struct device **__device_list_end;

void device_open(void)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list); i++) {
        if (__device_list[i]->opt.open) {
            __device_list[i]->opt.open();
        }
    }
}

void device_close(void)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list); i++) {
        if (__device_list[i]->opt.close) {
            __device_list[i]->opt.close();
        }
    }
}

int _read(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list); i++) {
        if (file == __device_list_end[file]->id) {
            if (__device_list[file]->opt.read) {
                return __device_list[file]->opt.read(ptr, len);
            }
            break;
        }
    }

    return -1;
}

int _write(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list); i++) {
        if (file == __device_list_end[file]->id) {
            if (__device_list[file]->opt.read) {
                return __device_list[file]->opt.write(ptr, len);
            }
            break;
        }
    }

    return -1;
}
