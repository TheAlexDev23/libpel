#! /bin/sh

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

# Building
cmake -S . -B build
make -C build

# Copying
cp include/pel.h /usr/include
cp build/libpel.a /usr/lib

echo "Install succesful"
