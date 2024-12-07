KERNEL_VERSION = $(shell uname -r)

LOCAL_KERNEL ?= /lib/modules/${KERNEL_VERSION}/build
OS_NAME = $(shell . /etc/os-release && echo $$ID)
OS_VERSION = $(shell . /etc/os-release && echo $$VERSION_ID)

all:
	@echo "Using OS: ${OS_NAME}"
	@echo "Using kernel directory: ${LOCAL_KERNEL}"
	@bash prebuild.sh
ifeq ($(OS_NAME), ubuntu)
ifeq ($(shell echo "${OS_VERSION} < 24.04" | bc -l),1)
	@bear $(MAKE) -C impl DEPS_LINUX=$(LOCAL_KERNEL)
else ifeq ($(OS_VERSION),24.04)
	@bear -- $(MAKE) -C impl DEPS_LINUX=$(LOCAL_KERNEL)
endif
else ifeq ($(OS_NAME), fedora)
	@bear -- $(MAKE) -C impl DEPS_LINUX=$(LOCAL_KERNEL)
endif
	@sed -i '/-mabi=lp64/d' compile_commands.json

.PHONY: clean

clean:
	@${MAKE} -C impl DEPS_LINUX=$(LOCAL_KERNEL) clean
	@sudo rm -rf $(LOCAL_KERNEL)/.config
