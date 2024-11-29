PWD := $(shell pwd)

obj-m := fake_driver.o

all:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) modules

clean:
	$(MAKE) -C $(DEPS_LINUX) M=$(PWD) clean
