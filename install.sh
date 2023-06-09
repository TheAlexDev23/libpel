#! /bin/sh

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

cp pel.pc /usr/share/pkgconfig

# Building
cmake -S . -B build
make -C build

# Copying
cp -r include/pel /usr/include
cp build/libpel.a /usr/lib

echo "Install succesful"
