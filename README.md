# PEL: Photo Editing Library

C library that allows png manipulation and image editing. Originally written for a school project.

## Installation

`install.sh` adds the pkgconf file of libpel to the system, configures the project and builts it.
After building, copies header and binaries into /usr/include/pel and /usr/bin.
Requires to be run with root privilegies.

```bash
git clone https://github.com/TheAlexDev23/libpel
cd libpel
chmod +x ./install.sh
sudo ./install.sh
```

## Testing

Assuming in project directory and already compiled/built the project
```bash
cd build
ctest --output-on-failure
```