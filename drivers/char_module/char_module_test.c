#include <stdio.h>
#if 1
#include <unistd.h>
#include <sys/ioctl.h>
#include "char_module_ioctl.h"
#endif

int main(int argc , char* argv[])
{
  if (argc != 2)
  {
    printf("usage: %s <device filename>\n", argv[0]);
    return -1;
  }
  FILE* fptr = fopen(argv[1], "r");
  if (fptr)
  {
#if 1
    ioctl(fileno(fptr), CHAR_MODULE_CMD_SET, 0xBADF00D);
    unsigned long data = ioctl(fileno(fptr), CHAR_MODULE_CMD_GET);
    printf("data read from char_module driver: 0x%08X\n", data);
    // there's a race condition between the printf above and the printk that
    // occurs in the driver when the fclose() below is called, so we sleep
    // here to avoid mangled terminal output
    sleep(1);
#endif
    fclose(fptr);
  }
}
