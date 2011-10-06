#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include "cdev_types.h"

#define MODULE_NAME "char_module_2"

// define a structure to hold driver info
typedef struct
{
  cdev_t cdev;
  dev_t dev;
  // add other members as needed for a given driver
} dev_config_t;

// driver function prototypes
static int     dev_create (dev_config_t* dev_conf_p, driver_funcs_t* funcs_p);

static driver_funcs_t driver_funcs =
{
  .owner          = THIS_MODULE,
};

static dev_config_t dev_config;

//-----------------------------------------------------------------------------

static int dev_create(dev_config_t* dev_config_p, driver_funcs_t* funcs_p)
{
    int ret = 0;

    printk("%s - %s: initializing module...\n", MODULE_NAME, __FUNCTION__);

    // dynamically allocate driver major/minor numbers - the device file 
    // must still be created (presumably by an init script)
    ret = alloc_chrdev_region(&dev_config_p->dev,  0, 1, MODULE_NAME);

    if (ret == 0)
    {
        printk("%s - %s: registering module...\n", MODULE_NAME, __FUNCTION__);

        // register the device driver - associate driver funcs w/device
        cdev_init(&dev_config_p->cdev, funcs_p);
        dev_config_p->cdev.ops = funcs_p;
        dev_config_p->cdev.owner = THIS_MODULE;
        ret = cdev_add(&dev_config_p->cdev, dev_config_p->dev, 1);
    }
    return ret;
}

//-----------------------------------------------------------------------------

int init_module(void)
{
  int status = 0;

  printk("%s - %s\n", MODULE_NAME, __FUNCTION__);

  status = dev_create(&dev_config, &driver_funcs);

  return status;
}

//-----------------------------------------------------------------------------

void cleanup_module(void)
{
  printk("%s - %s: unregistering module...\n", MODULE_NAME, __FUNCTION__);
  unregister_chrdev_region(dev_config.dev, 1);
  return;
}

//-----------------------------------------------------------------------------

MODULE_ALIAS("test:"MODULE_NAME);
MODULE_AUTHOR("Linux Student");
MODULE_DESCRIPTION("Basic Character Driver Kernel Module");
MODULE_LICENSE("GPL");
