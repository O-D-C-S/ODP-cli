# fastdriver
fastdriver - Command line [Fast-ODP](https://github.com/O-D-C-S/Old-Driver-Protocol) encrypt/decrypt tool

# Installation

## Requirements

- crypto++
- cmake
- pthread

## Build

```
cd src
mkdir build; cd build
cmake ..
make
```

# Usage

- `fastdriver -e [plain text] [key]`
- `fastdriver -d [encrypted text] [key]`
