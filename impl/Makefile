KBUILD_CFLAGS := -g -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs \
	-fno-strict-aliasing -fno-common \
	-Werror-implicit-function-declaration \
	-Wno-format-security \
	-fno-delete-null-pointer-checks -Wno-implicit-function-declaration \
	-Wno-unused-but-set-variable \
	-Wno-unused-local-typedefs -O2
	
PWD := $(shell pwd)
KERNEL_VERSION := $(shell uname -r)

obj-m := fake_driver.o

fake_driver-y := frdma_verb_ops.o

all:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) modules
	sudo mkdir -p /lib/modules/$(KERNEL_VERSION)/kernel/drivers/infiniband/hw/frdma
	sudo cp fake_driver.ko /lib/modules/$(KERNEL_VERSION)/kernel/drivers/infiniband/hw/frdma
	sudo depmod

clean:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) clean
	sudo rm -rf /lib/modules/$(KERNEL_VERSION)/kernel/drivers/infiniband/hw/frdma
