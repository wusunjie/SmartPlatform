#include <stdlib.h>

struct devopt {
    void (*open)(void *args);
    void (*close)(void);
    int (*read)(char *buf, int len);
    int (*write)(char *buf, int len);
};

struct device {
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

static struct uart_dev gpscom_dev = {
    .base.type = 0,
    .base.opt.open = NULL,
    .base.opt.close = NULL,
    .base.opt.read = NULL,
    .base.opt.write = NULL,
    .config.baudrate = 9600
};

static struct device *devlist[] = {
    (struct device *)&gpscom_dev
};

void device_open(void)
{
    int i;
    for (i = 0; i < sizeof(devlist) / sizeof(struct device *); i++) {
        switch (devlist[i]->type) {
            case 0:
            {
                struct uart_dev *dev = (struct uart_dev *)devlist[i];
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
    for (i = 0; i < sizeof(devlist) / sizeof(struct device *); i++) {
        switch (devlist[i]->type) {
            case 0:
            {
                if (devlist[i]->opt.close) {
                    devlist[i]->opt.close();
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
    if (file >= sizeof(devlist) / sizeof(struct device *)) {
        return -1;
    }

    switch (devlist[file]->type) {
        case 0:
        {
            if (devlist[file]->opt.read) {
                return devlist[file]->opt.read(ptr, len);
            }
        }
        break;
        default:
        break;
    }

    return -1;
}

int _write(int file, char *ptr, int len)
{
    if (file >= sizeof(devlist) / sizeof(struct device *)) {
        return -1;
    }

    switch (devlist[file]->type) {
        case 0:
        {
            if (devlist[file]->opt.write) {
                return devlist[file]->opt.write(ptr, len);
            }
        }
        break;
        default:
        break;
    }

    return -1;
}
