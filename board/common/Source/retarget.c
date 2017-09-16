#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <stdarg.h>

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

caddr_t _sbrk(int incr)
{
    extern char * end;
    extern char * _Main_Stack_Limit;
    static char * heap_end;
    char *        prev_heap_end;

    if (heap_end == NULL)
        heap_end = end;

    prev_heap_end = heap_end;

    if (heap_end + incr > _Main_Stack_Limit) {
      errno = ENOMEM;
      return (caddr_t) -1;
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
}

int _lseek(int file, int ptr, int dir)
{
    int i;

    for (i = 0; i < (__device_list_end - __device_list_begin); i++) {
        if (file == __device_list_begin[i]->id) {
            if (__device_list_begin[i]->opt.lseek) {
                return __device_list_begin[i]->opt.lseek(ptr, dir);
            }
            break;
        }
    }

    return -1;
}

int ioctl(int file, int request, ...)
{
    int i;
    va_list ap;
    int res = -1;

    for (i = 0; i < (__device_list_end - __device_list_begin); i++) {
        if (file == __device_list_begin[i]->id) {
            if (__device_list_begin[i]->opt.ioctl) {
                va_start(ap, request);
                res = __device_list_begin[i]->opt.ioctl(request, va_arg(ap, void *));
                va_end(ap);
            }
            break;
        }
    }

    return res;
}
