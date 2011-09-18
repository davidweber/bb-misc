/*
* Copyright (C) 2006 SigmaTel, Inc., David Weber <david@embeddedtoolbox.com>
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

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

