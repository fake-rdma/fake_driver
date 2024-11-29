#!/bin/bash

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

popd
