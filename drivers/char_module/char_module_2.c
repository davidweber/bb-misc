#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define MODULE_NAME "char_module"

// simplifying typedefs - move to .h file for easy re-use
typedef struct inode inode_t;
typedef struct file file_t;

// driver function prototypes
static int     dev_open   (inode_t* inodep, file_t* fp);
static int     dev_close  (inode_t* inodep, file_t* fp);

//-----------------------------------------------------------------------------

static int dev_open (inode_t* inodep, file_t* fp)
{
  printk("%s - %s\n", MODULE_NAME, __FUNCTION__);
  return 0;
}

//-----------------------------------------------------------------------------

static int dev_close (inode_t* inodep, file_t* fp)
{
  printk("%s - %s\n", MODULE_NAME, __FUNCTION__);
  return 0;
}

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


MODULE_ALIAS("test:"MODULE_NAME);
MODULE_AUTHOR("Linux Student");
MODULE_DESCRIPTION("Basic Character Driver Kernel Module");
MODULE_LICENSE("GPL");
