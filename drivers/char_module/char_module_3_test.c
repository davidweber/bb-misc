#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "char_module_ioctl.h"

int main(int argc , char* argv[])
{
  if (argc != 3)
  {
    printf("usage: %s <device filename> <value>\n", argv[0]);
    return -1;
  }
  FILE* fptr = fopen(argv[1], "r");
  if (fptr)
  {
    printf("writing data to char_module_3 driver: %d\n", atoi(argv[2]));
    sleep(1);
    ioctl(fileno(fptr), CHAR_MODULE_CMD_SET, atoi(argv[2]));
    printf("reading data from char_module_3 driver\n");
    sleep(1);
    unsigned long data = ioctl(fileno(fptr), CHAR_MODULE_CMD_GET);
    printf("data read from char_module driver: 0x%08X (%d)\n", data, data);
    // there's a race condition between the printf above and the printk that
    // occurs in the driver when the fclose() below is called, so we sleep
    // here to avoid mangled terminal output
    sleep(1);
    fclose(fptr);
  }
}
