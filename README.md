# PEL: Photo Editing Library

C library that allows png manipulation and image editing. Written for a school project, altough I might continue development.

## Installation

Will build and add pel.h to /usr/include and libpel.a to /usr/lib therefore requires root privilieges
```bash
git clone https://github.com/TheAlexDev23/libpel
cd libpel
chmod +x ./install.sh
sudo ./install.sh
```

## Testing

Assuming in project directory
```bash
cmake -S . -B build
cd build
make
ctest
```