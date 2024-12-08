#!/bin/bash

KERNEL_VERSION=$(uname -r)

LOCAL_KERNEL="/lib/modules/${KERNEL_VERSION}/build"
if [ ! -f "${LOCAL_KERNEL}/.config" ]; then
  sudo cp /boot/config-$KERNEL_VERSION ${LOCAL_KERNEL}/.config
fi

if [ ! -f $LOCAL_KERNEL/certs/signing_key.pem ]; then
  pushd $LOCAL_KERNEL/certs
  sudo openssl req -new -x509 -nodes -days 14 -subj "/CN=FRDMA module signing key" \
      -newkey rsa:2048 -keyout signing_key.x509 \
      -outform PEM -out signing_key.pem
  popd
fi
