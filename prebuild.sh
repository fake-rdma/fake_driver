#!/bin/bash

KERNEL_VERSION=$(uname -r)

if [ -d "/lib/modules/${KERNEL_VERSION}/build" ] && [ "$1" == "1" ]; then
  LOCAL_KERNEL="/lib/modules/${KERNEL_VERSION}/build"
  if [ ! -f "${LOCAL_KERNEL}/.config" ]; then
    sudo cp /boot/config-6.11.8-300.fc41.aarch64 ${LOCAL_KERNEL}/.config
  fi
  echo 1
else
  KDIR="$(pwd)/deps/linux-6.11.8"

  pushd deps

  if [ -d "linux-6.11.8" ]; then
    echo "Directory linux-6.11.8 already exists."
  else
    echo "Extracting linux-6.11.8.tar.gz..."
    if [ -f "linux-6.11.8.tar.gz" ]; then
      tar zxvf linux-6.11.8.tar.gz > /dev/null
    else
      echo "Error: linux-6.11.8.tar.gz not found."
      exit 1
    fi
    echo "Extraction complete."
  fi

  pushd linux-6.11.8

  echo $(pwd)
  if [ ! -f ".config" ]; then
    echo "start to build kernel"
    sudo cp ../../.config .
    sudo make -j4 > /dev/null
  else
    echo "already have RDMA config"
  fi

  popd
  popd

  echo 0
fi

if [ ! -f /usr/src/kernels/$KERNEL_VERSION/certs/signing_key.pem ]; then

  pushd /usr/src/kernels/$KERNEL_VERSION/certs

  sudo openssl req -new -x509 -nodes -days 14 -subj "/CN=FRDMA module signing key" \
      -newkey rsa:2048 -keyout signing_key.x509 \
      -outform PEM -out signing_key.pem

  popd

fi
