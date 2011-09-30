#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "omap3_beagle_ioctl.h"

int main(int argc , char* argv[])
{
  if (argc != 4)
  {
    printf("usage: %s <device filename> <addr> <data>\n", argv[0]);
    return -1;
  }
  FILE* fptr = fopen(argv[1], "r");
  uint8_t addr = 0;
  uint8_t data = 0;
  if ((sscanf(argv[2], "%hhx", &addr) != 1) ||
      (sscanf(argv[3], "%hhx", &data) != 1))
  {
    fprintf(stderr, "invalid argument\n");
    return -1;
  }
  if (fptr)
  {
    ioctl(fileno(fptr), OMAP3_BEAGLE_I2C_IOCT_WR, (data << 8) | addr);
    unsigned long data = ioctl(fileno(fptr), OMAP3_BEAGLE_I2C_IOCQ_RD);
    printf("data read from char_module driver: [0x%02X] = 0x%02X\n", addr, data);
    // there's a race condition between the printf above and the printk that
    // occurs in the driver when the fclose() below is called, so we sleep
    // here to avoid mangled terminal output
    sleep(1);
    fclose(fptr);
  }
}
