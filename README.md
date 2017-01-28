# fastdriver
[![Build Status](https://travis-ci.org/lackofdream/fastdriver.svg?branch=master)](https://travis-ci.org/lackofdream/fastdriver)

fastdriver - Command line [Fast-ODP](https://github.com/O-D-C-S/Old-Driver-Protocol) encrypt/decrypt tool

# Installation

## Requirements

- crypto++
- cmake
- pthread
- boost::program_options

## Build

```
cd src
mkdir build; cd build
cmake ..
make
```

# Usage
```
Usage: fastdriver [OPTION]... [FILE]
With no FILE, or when FILE is -, read standard input.

Options:
  -h [ --help ]         produce help message
  -e [ --encrypt ]      to encrypt message
  -d [ --decrypt ]      to decrypt message
  -k [ --key ] arg      key to encrypt/decrypt (required)
```