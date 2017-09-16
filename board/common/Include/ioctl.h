#ifndef _SMART_PLATFORM_IOCTL_H
#define _SMART_PLATFORM_IOCTL_H

#define IOCTL_REQ_GPRS_PWR_ON 0

int ioctl(int file,int request, ...);

#endif