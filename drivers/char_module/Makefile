STEP=
MODULE_NAME=char_module_$(STEP)
MODULE_FILE=$(MODULE_NAME).ko
ROOT_INSTALL_DIR=$(SDKDIR)/targetNFS/home/root
MKDEV_SCRIPT=$(HOME)/bb-misc/scripts/mkdev.sh
TEST_PROGRAM=$(MODULE_NAME)_test
CPP=$(TOOLCHAIN)/$(CROSS_COMPILE)g++

obj-m := $(MODULE_NAME).o

.PHONY: all install clean 

all: test
	make -C $(KERNELDIR) ARCH=arm M=$(PWD) modules

test:
ifeq ($(STEP),3)
	$(CPP) $(TEST_PROGRAM).c -o $(TEST_PROGRAM)
endif

install:
	cp -f $(MODULE_FILE) $(ROOT_INSTALL_DIR)
	cp -f $(MKDEV_SCRIPT) $(ROOT_INSTALL_DIR)
ifeq ($(STEP),3)
	cp -f $(TEST_PROGRAM) $(ROOT_INSTALL_DIR)
endif

clean:
	@rm -f *.ko *.symvers *.order *.o *.mod.* .*.cmd $(TEST_PROGRAM)
	@rm -rf .tmp*
	@rm -f $(MODULE_NAME)*_test

