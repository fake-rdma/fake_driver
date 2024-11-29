KDIR := $(shell pwd)/../deps/linux-6.11.8
PWD := $(shell pwd)

obj-m := fake_driver.o

all:
	@bear $(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
