#!/bin/bash

KERNEL_VERSION=$(uname -r)

LOCAL_KERNEL="/lib/modules/${KERNEL_VERSION}/build"
if [ ! -f "${LOCAL_KERNEL}/.config" ]; then
  sudo cp /boot/config-$KERNEL_VERSION ${LOCAL_KERNEL}/.config
fi
