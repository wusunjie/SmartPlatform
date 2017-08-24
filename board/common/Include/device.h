#ifndef _SMART_PLATFORM_DEVICE_H
#define _SMART_PLATFORM_DEVICE_H

struct devopt {
    void (*open)(void);
    void (*close)(void);
    int (*read)(char *buf, int len);
    int (*write)(char *buf, int len);
};

struct device {
    int id;
    struct devopt opt;
};

#define DEVICE_DEFINE(DEV, ID)   \
    static struct device DEV = { \
      .id = ID,                  \
      .opt.open = DEV##_open,    \
      .opt.close = DEV##_close,  \
      .opt.read = DEV##_read,    \
      .opt.write = DEV##_write   \
    };                           \
    static const struct device *DEV##_dev __attribute__((section(".device"))) = &DEV

#endif