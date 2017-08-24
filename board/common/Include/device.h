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

#endif