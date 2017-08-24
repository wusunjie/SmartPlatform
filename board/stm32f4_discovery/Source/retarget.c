#include <stdlib.h>

struct devopt {
    void (*open)(void);
    void (*close)(void);
    int (*read)(char *buf, int len);
    int (*write)(char *buf, int len);
};

struct device {
    int id;
    char type;
    struct devopt opt;
};

extern struct device **_device_list;
extern struct device **_device_list_end;

void device_open(void)
{
    int i;

    for (i = 0; i < (_device_list_end - _device_list) / sizeof(struct device *); i++) {
        if (_device_list[i]->opt.open) {
            _device_list[i]->opt.open();
        }
    }
}

void device_close(void)
{
    int i;

    for (i = 0; i < (_device_list_end - _device_list) / sizeof(struct device *); i++) {
        if (_device_list[i]->opt.close) {
            _device_list[i]->opt.close();
        }
    }
}

int _read(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < (_device_list_end - _device_list) / sizeof(struct device *); i++) {
        if (file == _device_list_end[file]->id) {
            if (_device_list[file]->opt.read) {
                return _device_list[file]->opt.read(ptr, len);
            }
            break;
        }
    }

    return -1;
}

int _write(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < (_device_list_end - _device_list) / sizeof(struct device *); i++) {
        if (file == _device_list_end[file]->id) {
            if (_device_list[file]->opt.read) {
                return _device_list[file]->opt.write(ptr, len);
            }
            break;
        }
    }

    return -1;
}
