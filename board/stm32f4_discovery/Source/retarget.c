#include <stdlib.h>

struct devopt {
    void (*open)(void *args);
    void (*close)(void);
    int (*read)(char *buf, int len);
    int (*write)(char *buf, int len);
};

struct device {
    int id;
    char type;
    struct devopt opt;
};

struct uart_config {
    int baudrate;
};

struct uart_dev {
    struct device base;
    struct uart_config config;
};

extern struct device **_device_list;
extern struct device **_device_list_end;

void device_open(void)
{
    int i;

    for (i = 0; i < (_device_list_end - _device_list) / sizeof(struct device *); i++) {
        switch (_device_list[i]->type) {
            case 0:
            {
                struct uart_dev *dev = (struct uart_dev *)_device_list[i];
                if (dev->base.opt.open) {
                    dev->base.opt.open(&(dev->config));
                }
            }
            break;
            default:
            break;
        }
    }
}

void device_close(void)
{
    int i;

    for (i = 0; i < (_device_list_end - _device_list) / sizeof(struct device *); i++) {
        switch (_device_list[i]->type) {
            case 0:
            {
                if (_device_list[i]->opt.close) {
                    _device_list[i]->opt.close();
                }
            }
            break;
            default:
            break;
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
        }
    }

    return -1;
}
