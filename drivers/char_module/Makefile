MODULE_NAME=char_module
MODULE_FILE=$(MODULE_NAME).ko
ROOT_INSTALL_DIR=$(SDKDIR)/targetNFS/home/root
MKDEV_SCRIPT=$(HOME)/bb-misc/scripts/mkdev.sh
TEST_PROGRAM=$(MODULE_NAME)_test
CPP=$(TOOLCHAIN)/$(CROSS_COMPILE)g++

obj-m := $(MODULE_NAME).o

.PHONY: all install clean 

all:
	make -C $(KERNELDIR) ARCH=arm M=$(PWD) modules
	$(CPP) $(TEST_PROGRAM).c -o $(TEST_PROGRAM)

install:
	cp -f $(MODULE_FILE) $(ROOT_INSTALL_DIR)
	cp -f $(MKDEV_SCRIPT) $(ROOT_INSTALL_DIR)
	cp -f $(TEST_PROGRAM) $(ROOT_INSTALL_DIR)

clean:
	@rm -f *.ko *.symvers *.order *.o *.mod.* .*.cmd $(TEST_PROGRAM)
	@rm -rf .tmp*
