PWD := $(shell pwd)

obj-m := fake_driver.o

SIGN_SYS := /usr/src/kernels/$(shell uname -r)

all:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) modules
	sudo $(SIGN_SYS)/scripts/sign-file sha512 $(SIGN_SYS)/certs/signing_key.x509 $(SIGN_SYS)/certs/signing_key.pem fake_driver.ko
	sudo mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/infiniband/hw/frdma
	sudo cp fake_driver.ko /lib/modules/$(shell uname -r)/kernel/drivers/infiniband/hw/frdma
	sudo depmod

clean:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) clean
	sudo rm -rf /lib/modules/$(shell uname -r)/kernel/drivers/infiniband/hw/frdma
