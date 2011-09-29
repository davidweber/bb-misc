#
# myapps/tlc59108-driver/Makefile
#

.PHONY: build install install_auto_load clean

TARGET_FILE = tlc59108-i2c
TEST_SCRIPT = tlc59108_i2c_test.sh
TARGET_DIR = ${shell pwd}
#KFLAGS=
#KERNELDIR=$(HOME)/beagleboard/sdk/psp/linux-2.6.39-r102h-arago1
CCFLAGS=ARCH=arm CROSS_COMPILE=arm-arago-linux-gnueabi-
FSROOT=$(HOME)/beagleboard/sdk/targetNFS
MODULE_DIR=$(FSROOT)/lib/modules/2.6.39/kernel/drivers/i2c

obj-m := $(TARGET_FILE).o

build:
	@echo "Building the kernel module"
	make -C $(KERNELDIR) ${CCFLAGS} M=$(TARGET_DIR) $(KFLAGS) modules
	@touch built

install: build
	@echo "Installing the module in $(FSROOT)"
	install -D -m 644 $(TARGET_DIR)/$(TARGET_FILE).ko $(FSROOT)/home/root/$(TARGET_FILE).ko 
	@echo "Installing the module in $(MODULE_DIR)"
	install -D -m 644 $(TARGET_DIR)/$(TARGET_FILE).ko $(MODULE_DIR)/$(TARGET_FILE).ko
	@echo "Installing $(TEST_SCRIPT)"
	install -D -m 755 $(TARGET_DIR)/$(TEST_SCRIPT) $(FSROOT)/home/root/$(TEST_SCRIPT)
	@touch installed

clean: 
	$(V) rm -rf *.symvers *.order *.ko *.debug *.o .*.d *.mod.c \
	.*.cmd .tmp_versions core *~ installed built $(QOUT)

