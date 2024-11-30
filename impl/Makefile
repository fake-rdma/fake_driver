PWD := $(shell pwd)

obj-m := fake_driver.o

SIGN_SYS := /usr/src/kernels/$(shell uname -r)

all:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) modules
	sudo $(SIGN_SYS)/scripts/sign-file sha512 $(SIGN_SYS)/certs/signing_key.x509 $(SIGN_SYS)/certs/signing_key.pem fake_driver.ko

clean:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) clean
