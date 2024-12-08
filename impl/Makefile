# KBUILD_CFLAGS := -g -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs \
# 	-fno-strict-aliasing -fno-common \
# 	-Werror-implicit-function-declaration \
# 	-Wno-format-security \
# 	-fno-delete-null-pointer-checks -Wno-implicit-function-declaration \
# 	-Wno-unused-but-set-variable \
# 	-Wno-unused-local-typedefs -O2
	
PWD := $(shell pwd)
KERNEL_VERSION := $(shell uname -r)
MODULE_TARGET := /lib/modules/$(KERNEL_VERSION)/kernel/drivers/infiniband/hw/frdma

obj-m := fake_driver.o

# fake_driver-y := frdma_verb_ops.o

all:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) modules
	@sudo $(DEPS_LINUX)/scripts/sign-file sha512 $(DEPS_LINUX)/certs/signing_key.x509 $(DEPS_LINUX)/certs/signing_key.pem fake_driver.ko
	@sudo mkdir -p $(MODULE_TARGET)
	@sudo cp $(PWD)/fake_driver.ko $(MODULE_TARGET)
	@sudo depmod

clean:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) clean
	@sudo rm -rf $(MODULE_TARGET)
