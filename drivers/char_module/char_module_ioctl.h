#ifndef _CHAR_MODULE_IOCTL_H_
#define _CHAR_MODULE_IOCTL_H_

#include <linux/ioctl.h>

#define CHAR_MODULE_MAGIC  0xC2

#define CHAR_MODULE_CMD_SET             _IOW(CHAR_MODULE_MAGIC, 0, long)
#define CHAR_MODULE_CMD_GET             _IOR(CHAR_MODULE_MAGIC, 1, long)

#endif //_CHAR_MODULE_IOCTL_H_

