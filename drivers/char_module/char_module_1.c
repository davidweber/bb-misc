#include <linux/module.h>
#include <linux/kernel.h>

#define MODULE_NAME "char_module_1"

//-----------------------------------------------------------------------------

int init_module(void)
{
  int status = 0;

  printk("%s - %s\n", MODULE_NAME, __FUNCTION__);

  return status;
}

//-----------------------------------------------------------------------------

void cleanup_module(void)
{
  printk("%s - %s\n", MODULE_NAME, __FUNCTION__);
  return;
}

//-----------------------------------------------------------------------------

MODULE_AUTHOR("Linux Student");
MODULE_DESCRIPTION("Basic Character Driver Kernel Module");
MODULE_LICENSE("GPL");
