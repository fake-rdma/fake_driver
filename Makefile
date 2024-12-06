KERNEL_VERSION = $(shell uname -r)

LOCAL_KERNEL ?= /lib/modules/${KERNEL_VERSION}/build
OS_NAME = $(shell . /etc/os-release && echo $$ID)

all:
	@echo "Using kernel directory: ${LOCAL_KERNEL}"
ifeq ($(OS_NAME), ubuntu)
	@bear $(MAKE) -C impl DEPS_LINUX=$(LOCAL_KERNEL)
else ifeq ($(OS_NAME), fedora)
	@bear -- $(MAKE) -C impl DEPS_LINUX=$(LOCAL_KERNEL)
endif
	@sed -i '/-mabi=lp64/d' compile_commands.json

.PHONY: clean

clean:
	@${MAKE} -C impl DEPS_LINUX=$(LOCAL_KERNEL) clean
	@sudo rm -rf $(LOCAL_KERNEL)/.config
