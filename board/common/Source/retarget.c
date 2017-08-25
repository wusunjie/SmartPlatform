#include <stdlib.h>

#include "device.h"

extern struct device *__device_list_begin[];
extern struct device *__device_list_end[];

void device_open(void)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list_begin); i++) {
        if (__device_list_begin[i]->opt.open) {
            __device_list_begin[i]->opt.open();
        }
    }
}

void device_close(void)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list_begin); i++) {
        if (__device_list_begin[i]->opt.close) {
            __device_list_begin[i]->opt.close();
        }
    }
}

int _read(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list_begin); i++) {
        if (file == __device_list_begin[i]->id) {
            if (__device_list_begin[i]->opt.read) {
                return __device_list_begin[i]->opt.read(ptr, len);
            }
            break;
        }
    }

    return -1;
}

int _write(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list_begin); i++) {
        if (file == __device_list_begin[i]->id) {
            if (__device_list_begin[i]->opt.read) {
                return __device_list_begin[i]->opt.write(ptr, len);
            }
            break;
        }
    }

    return -1;
}
