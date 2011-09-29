//------------------------------------------------------------------------------
// This file contains ioctl commands that control the Beagleboard-xM ULCD
//------------------------------------------------------------------------------

#ifndef _OMAP3_BEAGLE_LCD_IOCTL_H
#define _OMAP3_BEAGLE_LCD_IOCTL_H

#include <linux/ioctl.h>

#define OMAP3_BEAGLE_LCD_IOC_MAGIC  0xC1

//------------------------------------------------------------------------------
// IOCT = Tell = parameter contains info
//------------------------------------------------------------------------------
#define OMAP3_BEAGLE_LCD_IOCT_ON             _IO(OMAP3_BEAGLE_LCD_IOC_MAGIC,   0)
#define OMAP3_BEAGLE_LCD_IOCT_OFF            _IO(OMAP3_BEAGLE_LCD_IOC_MAGIC,   1)
#define OMAP3_BEAGLE_LCD_IOCT_TOGGLE         _IO(OMAP3_BEAGLE_LCD_IOC_MAGIC,   2)
#define OMAP3_BEAGLE_LCD_IOCT_FLIP           _IO(OMAP3_BEAGLE_LCD_IOC_MAGIC,   3)
#define OMAP3_BEAGLE_I2C_IOCT_WR             _IOW(OMAP3_BEAGLE_LCD_IOC_MAGIC,  4, long)
//------------------------------------------------------------------------------
// IOCQ = Query = return value contains info
//------------------------------------------------------------------------------
#define OMAP3_BEAGLE_I2C_IOCQ_RD             _IOR(OMAP3_BEAGLE_LCD_IOC_MAGIC, 5, long)

//------------------------------------------------------------------------------
// IOCS = Set = argument contains user space pointer to info
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IOCG = Get = return value contains info
//------------------------------------------------------------------------------

#endif //_OMAP3_BEAGLE_LCD_IOCTL_H

