/*
 *  tlc59108_i2c.c - Linux kernel module for the TLC59108 i2c LED driver.
 *
 *  Author: John Weber
 *  Copyright (C) 2011 Avnet Electronics Marketing
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 *  Note: 
 *	This is a client driver for the I2C bus only.  Insmoding this
 * 	driver will not necessarily result in a probe of the driver.
 *
 *	As most i2c devices are known to be resident on a particular
 *      bus at a particular i2c address, the i2c core will automatically
 *      instantiate a client driver if it sees a matching device on the
 *      bus.  The way to tell the i2c core about which devices to look
 *      on a particular bus is to add a member to an instance of the
 *	i2c_board_info structure, usually found in a board-specific 
 *      file in arch/<arch>/<mach>/<board_info.c>
 *
 *	For example, this driver was originally written for the 
 *	Beagleboard-xM.  The following lines would be added to the 
 *	kernel file:  arch/arm/mach-omap2/board-omap3beagle.c
 * static struct i2c_board_info __initdata beagle_i2c2_boardinfo[] = {
 * +       {
 * +               I2C_BOARD_INFO("tlc59108", 0x40),
 * +       },
 *
 * For more information see Documentation/i2c/instantiating-devices
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/i2c.h>
#include "omap3_beagle_ioctl.h"

/*
 *  TLC59108 device registers
 */

#define TLC59108_AI_NONE		(0x0 << 5)
#define TLC59108_AI_ALL			(0x4 << 5)
#define TLC59108_AI_PWMONLY             (0x5 << 5)
#define TLC59108_AI_CTLONLY             (0x6 << 5)
#define TLC59108_AI_CTLPWMONLY          (0x7 << 5)

#define TLC59108_REG_MODE1		(0x00)
#define TLC59108_REG_MODE2		(0x01)
#define TLC59108_REG_PWM0		(0x02)
#define TLC59108_REG_PWM1		(0x03)
#define TLC59108_REG_PWM2		(0x04)
#define TLC59108_REG_PWM3		(0x05)
#define TLC59108_REG_PWM4		(0x06)
#define TLC59108_REG_PWM5		(0x07)
#define TLC59108_REG_PWM6		(0x08)
#define TLC59108_REG_PWM7		(0x09)
#define TLC59108_REG_GRPPWM	 	(0x0A)
#define TLC59108_REG_GRPFREQ		(0x0B)
#define TLC59108_REG_LEDOUT0		(0x0C)
#define TLC59108_REG_LEDOUT1		(0x0D)
#define TLC59108_REG_SUBADR1		(0x0E)
#define TLC59108_REG_SUBADR2		(0x0F)
#define TLC59108_REG_SUBADR3		(0x10)
#define TLC59108_REG_ALLCALLADR		(0x11)
#define TLC59108_REG_IREF		(0x12)
#define TLC59108_REG_EFLAG		(0x13)

#define AI_MODE				TLC59108_AI_NONE

//#define DEBUG

#ifdef DEBUG
#define DPRINTK(fmt...) printk(fmt)
#else
#define DPRINTK(fmt...)
#endif

#define DRIVER_DESC "TLC59108 I2C driver"
#define DRIVER_NAME "tlc59108-i2c"

typedef struct inode inode_t;
typedef struct file file_t;
typedef struct semaphore semaphore_t;
typedef struct cdev cdev_t;
typedef struct file_operations driver_funcs_t;

typedef struct
{
    uint8_t  num_readers;
//    wait_queue_head_t wait_q;
    cdev_t cdev;
    dev_t dev;
    // add other members as needed for a given driver
    struct tlc59108_i2c *tlc59108_i2c_dev;;
} dev_config_t;

static int device_open (inode_t *inodep, file_t *filp);
static long device_ioctl (file_t *filp, unsigned int cmd, unsigned long arg);
static int device_release (inode_t *inodep, file_t *filp);
static int my_device_create(dev_config_t* devptr, driver_funcs_t* funcs);
static inline int tlc59108_i2c_write_reg(struct tlc59108_i2c *dev, u8 reg_addr, u8 data);
static inline int tlc59108_i2c_read_reg(struct tlc59108_i2c *dev, u8 reg_addr, u8* data);

static driver_funcs_t driver_funcs =
{
  .owner = THIS_MODULE,
  .unlocked_ioctl = device_ioctl,
  .open = device_open,
  .release = device_release
};

static dev_config_t dev_config;

static long device_ioctl (file_t *filp, unsigned int arg1, unsigned long arg2)
{
    u8 data = 0;
    u8 addr = 0;

    switch(arg1)
    {
      case OMAP3_BEAGLE_LCD_IOCT_ON:
        DPRINTK(DRIVER_DESC " %s(%s, 0x%08X)\n", __FUNCTION__, "OMAP_BEAGLE_LCD_IOCT_ON", arg2);
        tlc59108_i2c_write_reg(dev_config.tlc59108_i2c_dev, TLC59108_REG_LEDOUT0, 0x00);
        break;

      case OMAP3_BEAGLE_LCD_IOCT_OFF:
        DPRINTK(DRIVER_DESC " %s(%s, 0x%08X)\n", __FUNCTION__, "OMAP_BEAGLE_LCD_IOCT_OFF", arg2);
        tlc59108_i2c_write_reg(dev_config.tlc59108_i2c_dev, TLC59108_REG_LEDOUT0, 0x10);
        break;

      case OMAP3_BEAGLE_LCD_IOCT_TOGGLE:
        DPRINTK(DRIVER_DESC " %s(%s, 0x%08X)\n", __FUNCTION__, "OMAP_BEAGLE_LCD_IOCT_TOGGLE", arg2);
        tlc59108_i2c_read_reg(dev_config.tlc59108_i2c_dev, TLC59108_REG_LEDOUT0, &data);
        DPRINTK(DRIVER_DESC " data = 0x%02X\n", data);
        tlc59108_i2c_write_reg(dev_config.tlc59108_i2c_dev, TLC59108_REG_LEDOUT0, (data ^ 0x10));
        break;

      case OMAP3_BEAGLE_LCD_IOCT_FLIP:
        DPRINTK(DRIVER_DESC " %s(%s, 0x%08X)\n", __FUNCTION__, "OMAP_BEAGLE_LCD_IOCT_TOGGLE", arg2);
        tlc59108_i2c_read_reg(dev_config.tlc59108_i2c_dev, TLC59108_REG_LEDOUT1, &data);
        DPRINTK(DRIVER_DESC " data = 0x%02X\n", data);
        tlc59108_i2c_write_reg(dev_config.tlc59108_i2c_dev, TLC59108_REG_LEDOUT1, (data ^ 0x01));
        break;

      case OMAP3_BEAGLE_I2C_IOCQ_RD:
        DPRINTK(DRIVER_DESC " %s(%s, 0x%08X)\n", __FUNCTION__, "OMAP_BEAGLE_I2C_IOCQ_RD", arg2);
        addr = arg2;
        tlc59108_i2c_read_reg(dev_config.tlc59108_i2c_dev, addr, &data);
        DPRINTK(DRIVER_DESC " addr = 0x%02X, data = 0x%02X\n", addr, data);
        break;

      case OMAP3_BEAGLE_I2C_IOCT_WR:
        DPRINTK(DRIVER_DESC " %s(%s, 0x%08X)\n", __FUNCTION__, "OMAP_BEAGLE_I2C_IOCT_WR", arg2);
        data = (arg2 >> 8);
        addr = arg2;        
        DPRINTK(DRIVER_DESC " addr = 0x%02X, data = 0x%02X\n", addr, data);
        tlc59108_i2c_write_reg(dev_config.tlc59108_i2c_dev, addr, data);
        break;

      default:
        break;
    }
    return data;
}

static int device_open (inode_t *inodep, file_t *filp)
{
    filp->private_data = &dev_config;

    DPRINTK(KERN_INFO DRIVER_DESC " %s()\n", __FUNCTION__);
    return 0;
}

static int device_release (inode_t *inodep, file_t *filp)
{
    DPRINTK(KERN_INFO DRIVER_DESC " %s()\n", __FUNCTION__);
    return 0;
}

static int my_device_create(dev_config_t* dev_configp, driver_funcs_t* funcs)
{
    int ret = 0;

    DPRINTK(KERN_INFO "Initializing " DRIVER_DESC "\n");

    // initialize device configuration structure used by this driver
    dev_configp->num_readers = 0;

//    init_waitqueue_head(&dev_configp->wait_q);

    // dynamically allocate driver major/minor numbers - the device file 
    // must still be created (presumably by an init script)
    ret = alloc_chrdev_region(&dev_configp->dev,  0, 1, DRIVER_NAME);

    if (ret == 0)
    {
        DPRINTK(KERN_INFO "Registering " DRIVER_DESC "\n");

        // register the device driver - associate driver funcs w/ device
        cdev_init(&dev_configp->cdev, funcs);
        dev_configp->cdev.ops = funcs;
        dev_configp->cdev.owner = THIS_MODULE;
        ret = cdev_add(&dev_configp->cdev, dev_configp->dev, 1);
    }
    return ret;
}

struct tlc59108_i2c {
        char			name[I2C_NAME_SIZE];	
	struct i2c_client	*client;
	char			regaddr;  /* Addr of tlc59108 reg */
	char			regdata;  /* Data at regaddr */
};

/*
 *  Base i2c register read/write routines
 */

static inline int tlc59108_i2c_read_reg(struct tlc59108_i2c *dev, u8 reg_addr, u8* data)
{
	int ret;
	
	/*
          Provide the register addess within the fpga i2c slave
        */
	ret = i2c_master_send(dev->client, (u8*)&reg_addr, 1);
	if (ret < 0) {
                dev_err(&dev->client->dev, "i2c io (read) error: %d\n", ret);
                return ret;
        }

	/*
          Fetch the data
	*/
	ret = i2c_master_recv(dev->client, data, 1);
	if (ret < 0) {
		dev_err(&dev->client->dev, "i2c io (read) error: %d\n", ret);
		return ret;
	}

	dev_dbg(&dev->client->dev, "ret: 0x%x, data: 0x%x\n", ret, *data);
	
        DPRINTK(DRIVER_DESC " %s: addr = 0x%x, data = 0x%x\n", __FUNCTION__, reg_addr, *data);
	return ret;
}

static inline int tlc59108_i2c_write_reg(struct tlc59108_i2c *dev, u8 reg_addr, u8 data)
{
	u8 msg[2];

	/*
	 * Form a message of two bytes.  The first byte is the register address,
         * the second byte is the data to be written to the regiter.
	 */
	msg[0] = reg_addr;
	msg[1] = data;

	/*
	 *  Transmit the message to the device.
	 */
	return i2c_master_send(dev->client, msg, 2);
}

/*
 * SysFS support the 'regaddr' file.  Stores the value of the register to be
 * when the user reads or writes to the 'regdata' file.
 */

static ssize_t tlc59108_i2c_store_regaddr(struct device *dev, 
		struct device_attribute *attr, const char *buf, size_t count){

	struct i2c_client* client = to_i2c_client(dev);
	struct tlc59108_i2c *data = i2c_get_clientdata(client);
	
	/* Read the value in sysfs and convert to long */
	unsigned long val = simple_strtoul(buf, NULL, 16);

        data->regaddr = (unsigned char)(val & 0xFF);

	/* Update the client data structure */
	i2c_set_clientdata(client, data);

	return count;
}

static ssize_t tlc59108_i2c_show_regaddr(struct device *dev, 
		struct device_attribute *attr, char *buf){

	struct i2c_client* client = to_i2c_client(dev);
	struct tlc59108_i2c *data = i2c_get_clientdata(client);

	/* Show the value of regaddr */
	return sprintf(buf, "%x\n",data->regaddr);
}

static DEVICE_ATTR(regaddr, S_IWUSR | S_IRUGO, 
		tlc59108_i2c_show_regaddr, tlc59108_i2c_store_regaddr);

/*
 * SysFS support the 'regdata' file.  Reads or writes data to the fpga i2c
 * register at address 'regaddr'.
 */

static ssize_t tlc59108_i2c_store_regdata(struct device *dev, 
		struct device_attribute *attr, const char *buf, size_t count){

	struct i2c_client* client = to_i2c_client(dev);
	struct tlc59108_i2c *data = i2c_get_clientdata(client);
	
	/* Read the value in sysfs and convert to long */
	unsigned long val = simple_strtoul(buf, NULL, 16);

        data->regdata = (unsigned char)(val & 0xFF);

	/* Write the register.  The address of the register to write
           is stored in data->regaddr */
	tlc59108_i2c_write_reg(data, data->regaddr, data->regdata);
	
	/* Update the client data structure */
	i2c_set_clientdata(client, data);

	return count;
}


static ssize_t tlc59108_i2c_show_regdata(struct device *dev, 
		struct device_attribute *attr, char *buf){

	u8 readval = 0;
	struct i2c_client* client = to_i2c_client(dev);
	struct tlc59108_i2c *data = i2c_get_clientdata(client);

	/* Get the register value.  The address of the register to read
           is in data->regaddr */
	tlc59108_i2c_read_reg(data, data->regaddr, &readval);

	data->regdata = readval & 0xFF;
	
	/* Update the client data structure */
	i2c_set_clientdata(client, data);

	return sprintf(buf, "%x\n",data->regdata);
}

static DEVICE_ATTR(regdata, S_IWUSR | S_IRUGO, 
		tlc59108_i2c_show_regdata, tlc59108_i2c_store_regdata);


/*
 *  Initialize the attributes
 */

static struct attribute *tlc59108_i2c_attributes[] = {
	&dev_attr_regaddr.attr,
	&dev_attr_regdata.attr,
	NULL
};

static const struct attribute_group tlc59108_i2c_attr_group = {
	.attrs = tlc59108_i2c_attributes,
};


/*
 *  Probe function - inits the driver.
 */

static int __devinit tlc59108_i2c_probe(struct i2c_client *new_client,
	  			   const struct i2c_device_id *id)
{
	struct tlc59108_i2c *dev;
	int err;
         
        DPRINTK("%s: entered\n", __FUNCTION__);

	if (!i2c_check_functionality(new_client->adapter,
				     I2C_FUNC_I2C))
		return -EIO;

	// Allocate some kernel memory for the tlc59108_i2c structure
	dev = kzalloc(sizeof(struct tlc59108_i2c), GFP_KERNEL);
	if (!dev) {
		err = -ENOMEM;
		goto err_free_mem;
	}

	// Assign structure members
	dev->client = new_client;

        // keep a handle for use by file ops
        dev_config.tlc59108_i2c_dev = dev;

	// Initialize certain struct data
	dev->regaddr = 0;

	snprintf(dev->name, sizeof(dev->name),
		 "%s", dev_name(&new_client->dev));

	i2c_set_clientdata(new_client, dev);

	err = sysfs_create_group(&new_client->dev.kobj, &tlc59108_i2c_attr_group);
	if (err)
		goto err_free_mem;

	return 0;

err_free_mem:
	kfree(dev);
	return err;
}

static int __devexit tlc59108_i2c_remove(struct i2c_client *client)
{
        struct tlc59108_i2c *dev;

      	DPRINTK("%s: entered\n",__FUNCTION__);
	dev_dbg(&client->dev,"%s: dev_dbg message\n",__FUNCTION__);

	dev = i2c_get_clientdata(client);

        sysfs_remove_group(&client->dev.kobj, &tlc59108_i2c_attr_group);

	kfree(dev);

	return 0;
}

static struct i2c_device_id tlc59108_i2c_idtable[] = {
	{ "tlc59108", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, tlc59108_i2c_idtable);

static struct i2c_driver tlc59108_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "tlc59108"
	},
	.id_table	= tlc59108_i2c_idtable,
	.probe		= tlc59108_i2c_probe,
	.remove		= __devexit_p(tlc59108_i2c_remove),
};

static int __init tlc59108_i2c_init(void)
{
    int ret = 0;

    DPRINTK("%s: entered\n", __FUNCTION__);

    ret = my_device_create(&dev_config, &driver_funcs);

    if (ret == 0)
    {
        // initialize hardware
        DPRINTK(KERN_INFO "SUCCESS - major:%d, minor:%d\n",
               MAJOR(dev_config.dev), MINOR(dev_config.dev));

        ret = i2c_add_driver(&tlc59108_i2c_driver);
    }
    else
    {
        DPRINTK(KERN_INFO "FAILURE\n");
    }
    DPRINTK("%s: exiting, ret = %d\n", __FUNCTION__, ret);
    return ret;
}

static void __exit tlc59108_i2c_exit(void)
{
	i2c_del_driver(&tlc59108_i2c_driver);
}

module_init(tlc59108_i2c_init);
module_exit(tlc59108_i2c_exit);

MODULE_AUTHOR("John Weber <john.weber@avnet.com");
MODULE_DESCRIPTION("TLC59108 I2C Driver");
MODULE_LICENSE("GPL");
