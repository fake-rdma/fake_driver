KERNEL_VERSION = $(shell uname -r)

REMOTE_KERNEL := $(shell pwd)/deps/linux-6.11.8/
LOCAL_KERNEL ?= /lib/modules/${KERNEL_VERSION}/build  

DEPS_LINUX ?= $(REMOTE_KERNEL)
IS_LOCAL ?= $(shell ./prebuild.sh)
IS_OK := 1

# prepare_bash:
	
ifeq ($(IS_OK),$(IS_LOCAL))
	DEPS_LINUX := $(LOCAL_KERNEL)
else
	DEPS_LINUX := $(REMOTE_KERNEL)
endif

all:
	@echo "Using kernel directory: ${DEPS_LINUX}"
	@echo "Using DEPS_LINUX=${DEPS_LINUX}"
	@bear -- $(MAKE) -C impl DEPS_LINUX=$(DEPS_LINUX)

.PHONY: clean

clean:
	${MAKE} -C impl DEPS_LINUX=$(DEPS_LINUX) clean
	sudo ${MAKE} -C $(DEPS_LINUX) clean
